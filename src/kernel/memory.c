/*
   Copyright [2021] [juwonpee]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	 http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/



#include "memory.h"


void* heap_start;
void* heap_end;

volatile memory_pageDirectoryCR3_t pageDirectoryCR3;
alignas(4096) memory_pageDirectory_t pageDirectory[1024];
alignas(4096) memory_pageTable_t pageTable[1024][1024];

memory_page_descriptor_t page_descriptor[1024][1024];

void memset(void* address, char value, uintptr_t size) {
	char* destIndex_char = (char*)address;
	for (uintptr_t index = 0; index < size; index++) {
		destIndex_char[index] = value;        
	}
}

void memcpy(void* dest, void* src, uintptr_t size) {
	char* srcIndex = (char*)src;
	char* destIndex = (char*)dest;

	for (uintptr_t index = 0; index < size; index++) {
		destIndex[index] = srcIndex[index];
	}
}

void* malloc(uintptr_t size) {
	// Kernel critical section
	scheduler_kernel_lock();

	// malloc now respects page boundaries
	// All memory allocations are word size aligned for speed;
	if (size <= 0xFFFFFFFC) {
		if (size % 4) {
			size = size & 0xFFFFFFFC;
			size += 4;
		}
	}
	else {
		println("Memory allocation request too large!");
		panic();
	}

	memory_malloc_node_t* node = heap_start;
	memory_malloc_node_t* lastNode = node;
	while (1) {
		// Check if the page of current node pointer exists (preventative page fault checks)
		if (!memory_kernel_check_exists(node)) {
			memory_kernel_page_alloc(node);
		}

		// Check if last node
		if (node->nextNode == 0) {
			// Add references to current page
			uintptr_t i = ((uintptr_t)node & 0xFFC00000) >> 22;
			uintptr_t j = ((uintptr_t)node & 0x3FF000) >> 12;
			page_descriptor[i][j].references++;

			// Add nodes and allocate new pages if necessary
			if ((uintptr_t)node + size + sizeof(memory_malloc_node_t) < ((uintptr_t)node & 0xFFFFF000) + 0x1000) {		// Fits within current page
				// Insert new node to end of list
				node->size = size + sizeof(memory_malloc_node_t);
				node->prevNode = lastNode;
				node->nextNode = (memory_malloc_node_t*)nullptr;
				lastNode->nextNode = node;

				scheduler_kernel_unlock();
				return &node->data;
			}
			else {																				// Crosses over page, need to allocate new page
				// Allocate respective pages and references
				// Check how many pages this malloc crosses
				uintptr_t nodePage = (uintptr_t)node;
				nodePage += size + sizeof(memory_malloc_node_t);
				uintptr_t numPages = (nodePage & 0xFFFFF000) - ((uintptr_t)node & 0xFFFFF000);
				for (uintptr_t pageIndex = (uintptr_t)node & 0xFFFFF000; pageIndex <= pageIndex + numPages * 0x1000; pageIndex += 0x1000) {
					uintptr_t i = (pageIndex & 0xFFC00000) >> 22;
					uintptr_t j = (pageIndex & 0x3FF000) >> 12;
					memory_kernel_page_alloc((void*)pageIndex);
					page_descriptor[i][j].references++;
				}

				// Insert new node to end of list
				node->size = size + sizeof(memory_malloc_node_t);
				node->prevNode = lastNode;
				node->nextNode = (memory_malloc_node_t*)nullptr;
				lastNode->nextNode = node;

				scheduler_kernel_unlock();
				return &node->data;
			}
		}
		else if((uintptr_t)node->nextNode - (uintptr_t)node - node->size >= size + sizeof(memory_malloc_node_t)) {	// Check if space in between nodes are large enough
			memory_malloc_node_t* tempNextNode = node->nextNode;
			memory_malloc_node_t* tempPrevNode = node;

			// setup the node in between the two nodes
			node = (memory_malloc_node_t*)((uintptr_t)node + node->size);
			node->size = size + sizeof(memory_malloc_node_t);
			node->nextNode = tempNextNode;
			node->prevNode = tempPrevNode;

			// Change next node pointers
			tempNextNode->prevNode = node;

			// Change prev node pointers
			tempPrevNode->nextNode = node;

			// Add references to respective pages
			// Check how many pages this malloc crosses
			uintptr_t nodePage = (uintptr_t)node;
			nodePage += size + sizeof(memory_malloc_node_t);
			uintptr_t numPages = (nodePage & 0xFFFFF000) - ((uintptr_t)node & 0xFFFFF000);
			for (uintptr_t pageIndex = (uintptr_t)node & 0xFFFFF000; pageIndex <= pageIndex + numPages * 0x1000; pageIndex += 0x1000) {
				uintptr_t i = (pageIndex & 0xFFC00000) >> 22;
				uintptr_t j = (pageIndex & 0x3FF000) >> 12;
				page_descriptor[i][j].references++;
			}

			scheduler_kernel_unlock();
			return &node->data;
		}
		lastNode = node;
		node = (memory_malloc_node_t*)(node->size + (uintptr_t)node);
	}
}

void free(void* address) {
	// Kernel critical section
	scheduler_kernel_lock();

	memory_malloc_node_t* node = (memory_malloc_node_t*)((uintptr_t)address - sizeof(memory_malloc_node_t));
	
	// Remove references from pages
	uintptr_t nodePage = (uintptr_t)node;
	nodePage += node->size + sizeof(memory_malloc_node_t);
	uintptr_t numPages = (nodePage & 0xFFFFF000) - ((uintptr_t)node & 0xFFFFF000);
	for (uintptr_t pageIndex = (uintptr_t)node & 0xFFFFF000; pageIndex <= pageIndex + numPages * 0x1000; pageIndex += 0x1000) {
		uintptr_t i = (pageIndex & 0xFFC00000) >> 22;
		uintptr_t j = (pageIndex & 0x3FF000) >> 12;
		page_descriptor[i][j].references--;
		memory_kernel_page_free((void*)pageIndex);
	}

	// Connect prev node and next node
	memory_malloc_node_t* prevNode = node->prevNode;
	memory_malloc_node_t* nextNode = node->nextNode;
	prevNode->nextNode = nextNode;
	nextNode->prevNode = prevNode;

	// Clear data
	for (uintptr_t nodeIndex = 0; nodeIndex < node->size; nodeIndex++) {
		node->data[nodeIndex] = 0;
	}

	node->size = 0;
	node->nextNode = (memory_malloc_node_t*)nullptr;
	node->prevNode = (memory_malloc_node_t*)nullptr;

	scheduler_kernel_unlock();
}




void memory_interrupt_handler(IDT_pageFault_error_t pageFault_error, void* address) {
	if (pageFault_error.U == 0) {
		if (pageFault_error.P == 0)	{															// In kernel context, assign all memory
			memory_kernel_page_alloc(address);
		}
	}
	else {

	}
}

void memory_kernel_page_alloc(void* address) {
	// Kernel critical section
	scheduler_kernel_lock();
	
	// Check if kernel page is already allocated
	uintptr_t i = ((uintptr_t)address & 0xFFC00000) >> 22;
	uintptr_t j = ((uintptr_t)address & 0x3FF000) >> 12;
	if (page_descriptor[i][j].pageTable->present == 0) {
		// Find free page
		while (1) {

		}
	}

	scheduler_kernel_unlock();
}

void memory_kernel_page_free(void* address) {
	// Kernel critical section
	scheduler_kernel_lock();

	
	
	scheduler_kernel_unlock();
}

bool memory_kernel_check_exists(void* address) {
	scheduler_kernel_lock();

	scheduler_kernel_unlock();
}

bool memory_init(struct multiboot_tag_basic_meminfo* multiboot_meminfo, void* heapStart) {
/*-----------------------------------------------------------------------------------------------*/
/*                                          Memory Type                                          */
/*-----------------------------------------------------------------------------------------------*/

	void* kernel_start_page = (void*)(multiboot_meminfo->mem_lower * 1024);
	void* heap_start_page = heapStart;
	void* heap_end_page = (void*) (multiboot_meminfo->mem_upper * 1024 + (uint32_t)kernel_start_page);
	heap_start = heap_start_page;
	heap_end = heap_end_page - 1;                                                               // Leave last page free cuz lazy to do all the checking

	// malloc init, clear multiboot2 information and other data
	memset(heap_start_page, 0, (uintptr_t)heap_end_page + 4095 - (uintptr_t) heap_start_page);              
	uintptr_t* tempFix = malloc(4);																// TEMPFIX: Because of the first node problem of malloc, need an unreferenced first node to keep the references for the second node
	(*tempFix) = 0x12345678;

/*-----------------------------------------------------------------------------------------------*/
/*                                          Paging Init                                          */
/*-----------------------------------------------------------------------------------------------*/

	// Initialization of paging structures
	// Only enable kernel space memory for now, enable MMIO and user space later in init process

	// Init pageDirectoryCR3
	pageDirectoryCR3.address = (uint32_t)&pageDirectory >> 12;
	pageDirectoryCR3.pageWriteThrough = 0;
	pageDirectoryCR3.pageCacheDisable = 0;

	// Init pageDirectory
	{
		void* currentPageIndex;
		void* heapStartPageDirectory;
		uintptr_t temp = (uintptr_t)heap_start_page;
		if (temp & 0x400000) {
			temp &= 0xFF400000;
			temp += 0x400000;
		}
		heapStartPageDirectory = (void*) temp;
		for (uintptr_t i = 0; i < 1024; i++) {
			currentPageIndex = (void*)(i * 1024 * 4096);
			if (currentPageIndex <= heapStartPageDirectory) {                                   // Kernel space
				currentPageIndex = (void*)(i * 1024 * 4096);
				pageDirectory[i].present = 1;
				pageDirectory[i].RW = 1;
				pageDirectory[i].US = 0;
				pageDirectory[i].pageWriteThrough = 0;
				pageDirectory[i].pageCacheDisable = 0;
				pageDirectory[i].size = 1;
				pageDirectory[i].address = (uint32_t)&pageTable[i] >> 12;
				for (uintptr_t j = 0; j < 1024; j++) {
					currentPageIndex = (void*)(i * 1024 * 4096 + j * 4096);
					pageTable[i][j].present = 1;
					pageTable[i][j].RW = 1;
					pageTable[i][j].US = 0;
					pageTable[i][j].pageWriteThrough = 0;
					pageTable[i][j].pageCacheDisable = 0;
					pageTable[i][j].global = 0;													// Will set to global cuz lazy to implement TLBs
					pageTable[i][j].address = ((uint32_t)currentPageIndex) >> 12;

					// Page descriptor stuff
					page_descriptor[i][j].pid = SCHED_PID_KERNEL;
					page_descriptor[i][j].pageTable = &pageTable[i][j];
					page_descriptor[i][j].references = 0xFFFFFFFF;

				}
			}
			else {
				pageDirectory[i].present = 0;

				// Page descriptor stuff
				for (uintptr_t j = 0; j < 1024; j++) {
					page_descriptor[i][j].pid = SCHED_PID_NOTHING;
					page_descriptor[i][j].pageTable = &pageTable[i][j];
					page_descriptor[i][j].references = 0;
				}
			}
		}
	}


/*-----------------------------------------------------------------------------------------------*/
/*                                         Enable Paging                                         */
/*-----------------------------------------------------------------------------------------------*/

// Move page directory structure to CR3
// Enable paging
	asm volatile (
		"mov %0, %%eax              \n\t"
		"mov %%eax, %%cr3"

		: 
		: "r" (pageDirectoryCR3)
	);
	asm volatile (
		"mov %cr0, %eax           \n\t"
		"or $0x80000001, %eax      \n\t"
		"mov %eax, %cr0"
	);
	
	return false;
}
