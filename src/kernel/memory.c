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
alignas(4096) volatile memory_pageDirectory_t pageDirectory[1024];
alignas(4096) volatile memory_pageTable_t pageTable[1024][1024];

memory_page_descriptor_t page_descriptor[1024][1024];

void* malloc(uintptr_t size) {
	// malloc now 

	// // TODO: If first node gets freed, everything breaks cuz the references to the second node are broken

	// // All memory allocations are word size aligned cuz speed
	// if (size <= 0xFFFFFFFC) {
	// 	if (size % 0x4) {
	// 		size = size & 0xFFFFFFFC;
	// 		size += 4;
	// 	}
	// }
	// else {
	// 	println("Memory alloc request too large");
	// 	panic();
	// }

	// memory_malloc_node_t* node = heap_start;
	// memory_malloc_node_t* lastNode = node;
	// while (1) {
	// 	if (node->size == 0) {
	// 		// Insert new node to end of list
	// 		node->size = size + sizeof(memory_malloc_node_t);
	// 		if (lastNode != node) {
	// 			node->prevNode = lastNode;
	// 			lastNode->nextNode = node;
	// 		}
	// 		return &node->data;
	// 	}
	// 	else {
	// 		if (node->nextNode != 0) {
	// 			if ((uintptr_t)node->nextNode - (uintptr_t)node - node->size >= size + sizeof(memory_malloc_node_t)) {
	// 				memory_malloc_node_t* tempNextNode = node->nextNode;
	// 				memory_malloc_node_t* tempPrevNode = node;

	// 				// setup the node in between the two nodes
	// 				node = (memory_malloc_node_t*)((uintptr_t)node + node->size);
	// 				node->size = size + sizeof(memory_malloc_node_t);
	// 				node->nextNode = tempNextNode;
	// 				node->prevNode = tempPrevNode;

	// 				// Change next node pointers
	// 				tempNextNode->prevNode = node;

	// 				// Change prev node pointers
	// 				tempPrevNode->nextNode = node;

	// 				return &node->data;
	// 			}
	// 		}
	// 		lastNode = node;
	// 		node = (memory_malloc_node_t*)(node->size + (uintptr_t)node);
	// 	}
	// }
}

void free(void* address) {
	memory_malloc_node_t* node = address - sizeof(memory_malloc_node_t);
	memory_malloc_node_t* prevNode = node->prevNode;
	memory_malloc_node_t* nextNode = node->nextNode;
	for (uintptr_t index = 0; index < (node->size - sizeof(memory_malloc_node_t)) / 4; index++) {
		node->data[index] = 0;
	}
	// clear remaining data of current node
	node->size = 0;
	node->prevNode = 0;
	node->nextNode = 0;

	// Connect prev node and next node
	prevNode->nextNode = nextNode;
	nextNode->prevNode = prevNode;
}

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

	uint32_t cr2_pageDirectory = (uint32_t)address >> 22;
	uint32_t cr2_pageTable = ((uint32_t)address << 10) >> 22;


	// Find free page
	for (uintptr_t x = 0; x < 1024 * 1024; x++) {
		uintptr_t i = x / 1024;
		uintptr_t j = x % 1024;
		if (page_descriptor[i][j].references == 0) {
			page_descriptor[i][j].references = 1;
			page_descriptor[i][j].pid = SCHED_PIC_KERNEL;
			page_descriptor[i][j].virtual_address = (void*)((uintptr_t)address >> 12 << 12);
			page_descriptor[i][j].physical_address = (void*)(i * 1024 * 4096 + j * 4096);

			// Fill page table and page directories
			if (pageDirectory[cr2_pageDirectory].present != 1) {
				pageDirectory[cr2_pageDirectory].RW = 1;
				pageDirectory[cr2_pageDirectory].US = 0;
				pageDirectory[cr2_pageDirectory].pageWriteThrough = 0;
				pageDirectory[cr2_pageDirectory].pageCacheDisable = 0;
				pageDirectory[cr2_pageDirectory].size = 1;
				pageDirectory[cr2_pageDirectory].address = (uint32_t)&pageTable[cr2_pageDirectory] >> 12;
			}

			pageTable[cr2_pageDirectory][cr2_pageTable].present = 1;
			pageTable[cr2_pageDirectory][cr2_pageTable].RW = 1;
			pageTable[cr2_pageDirectory][cr2_pageTable].US = 0;
			pageTable[cr2_pageDirectory][cr2_pageTable].pageWriteThrough = 0;
			pageTable[cr2_pageDirectory][cr2_pageTable].pageCacheDisable = 0;
			pageTable[cr2_pageDirectory][cr2_pageTable].global = 0;
			pageTable[cr2_pageDirectory][cr2_pageTable].address = (i * 1024 * 4096 + j * 4096) >> 12;
		}
	}
	

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
					page_descriptor[i][j].references = 0xFFFFFFFF;
					page_descriptor[i][j].pid = SCHED_PIC_KERNEL;
					page_descriptor[i][j].physical_address = (uint32_t)currentPageIndex;
					page_descriptor[i][j].virtual_address = (uint32_t)currentPageIndex;
				}
			}
			else {
				pageDirectory[i].present = 0;

				// Page descriptor stuff
				for (int j = 0; j < 1024; j++) {
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


	// Make a linked list of page descriptors for the kernel
	//page_descriptor = malloc(sizeof(memory_page_descriptor_t));
	for (int i = 0; i < 1024; i++) {
		if (pageDirectory[i].present == 1) {
			for (int j = 0; j < 1024; j++) {
				if (pageTable[i][j].present == 1) {
					page_descriptor[i][j].references = 1;
					page_descriptor[i][j].pid = SCHED_PIC_KERNEL;
					page_descriptor[i][j].virtual_address = (void*)(pageTable[i][j].address << 12);
					page_descriptor[i][j].physical_address = (void*)(pageTable[i][j].address << 12);
				}
				else {
					page_descriptor[i][j].references = 0;
				}
			}
		}
		else {
			page_descriptor[i][0].references = 0;
		}
	}
	
	return false;
}
