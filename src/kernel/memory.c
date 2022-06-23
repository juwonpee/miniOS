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


void* heapEnd;
memoryMallocNode_t* firstNode;

volatile memoryPageDirectoryCR3_t pageDirectoryCR3;
alignas(4096) memoryPageDirectory_t pageDirectory[1024];
alignas(4096) memoryPageTable_t pageTable[1024][1024];

memoryProcessMemoryDescriptor_t processMemoryDescriptor[1024];

void memset(void* address, char value, uintptr_t size) {
	char* dest_char = (char*)address;

	if (size <= 0x4) {
		for (uintptr_t index = 0; index < size; index++) {
			dest_char[index] = value;
		}
	}
	else {
		uintptr_t* dest_word = (uintptr_t*)address;
		uintptr_t value_word = (value << 24) + (value << 16) + (value << 8) + value;

		for (uintptr_t size_word = 0; size_word < size / 4; size_word++) {
			dest_word[size_word] = value_word;
		}
		dest_char = (char*)(dest_word + size / 4);
		for (uintptr_t size_char = 0; size_char < size % 4; size_char++) {
			dest_char[size_char] = value;
		}
	}
}

void memcpy(void* dest, void* src, uintptr_t size) {
	char* srcIndex = (char*)src;
	char* destIndex = (char*)dest;

	for (uintptr_t index = 0; index < size; index++) {
		destIndex[index] = srcIndex[index];
	}
}

void* kmalloc(uintptr_t size) {
	// Kernel critical section
	scheduler_kernel_uninterruptible();

	// Check for boundary sizes
	if (size > MEM_KERNEL_SIZE) {
		panic("Malloc Error: Memory allocation too large");
	}
	else if (size == 0) {
		return nullptr;
	}
	
	// Find next free node
	memoryMallocNode_t* node = firstNode;
	memoryMallocNode_t* prevNode;
	memoryMallocNode_t* nextNode;
	while ((uintptr_t)node + sizeof(memoryMallocNode_t) + size < MEM_USR_START) {	// While node is within memory boundaries
		if (node->next != nullptr) { // If not empty node
			// Check space between current and next node
			if ((uintptr_t)node->next - (uintptr_t)node - size - sizeof(memoryMallocNode_t) > 0) {	// If space is big enough
				// assign space inbetween node
				prevNode = node;
				nextNode = node->next;
				node = node + sizeof(memoryMallocNode_t) + size;
				break;
			}
			node = node->next;
		}
	}

	if ((uintptr_t)node + sizeof(memoryMallocNode_t) + size >=  MEM_USR_START) panic("Not enough memory");

	prevNode->next = node;
	nextNode->prev = node;
	node->size = size + sizeof(memoryMallocNode_t);
	node->prev = prevNode;
	node->next = nextNode;

	// End critical section
	scheduler_kernel_interruptible();

	return &node->data;
}

void* kmalloc_align(uintptr_t size, uintptr_t align) {
	// Kernel critical section
	scheduler_kernel_uninterruptible();

		// Check for boundary sizes
	if (size > MEM_KERNEL_SIZE) {
		panic("Malloc Error: Memory allocation too large");
	}
	else if (size == 0) {
		return nullptr;
	}
	
	// Find next free node
	memoryMallocNode_t* node = firstNode;
	memoryMallocNode_t* prevNode;
	memoryMallocNode_t* nextNode;
	while ((uintptr_t)node + sizeof(memoryMallocNode_t) + size < MEM_USR_START) {	// While node is within memory boundaries
		if (node->next != nullptr) { // If not empty node
			// Check space between current and next node
			if ((uintptr_t)node->next - (uintptr_t)node - size - sizeof(memoryMallocNode_t) > 0) {	// If space is big enough
				// assign space inbetween node
				prevNode = node;
				nextNode = node->next;
				node = node + sizeof(memoryMallocNode_t) + size;
				break;
			}
			node = node->next;
		}
	}

	if ((uintptr_t)node + sizeof(memoryMallocNode_t) + size >=  MEM_USR_START) panic("Not enough memory");

	prevNode->next = node;
	nextNode->prev = node;
	node->size = size + sizeof(memoryMallocNode_t);
	node->prev = prevNode;
	node->next = nextNode;

	// End critical seciton
	scheduler_kernel_interruptible();
}


void kfree(void* freeNode) {
	memoryMallocNode_t* node = freeNode - sizeof(memoryMallocNode_t);
	memoryMallocNode_t* prev = node->prev;
	memoryMallocNode_t* next = node->next;

	// Join prev and next nodes
	prev->next = next;
	next->prev = prev;

	// delete node
	node->size = 0;
	node->prev = nullptr;
	node->next = nullptr;
}

bool memory_init(struct multiboot_tag_basic_meminfo* multiboot_meminfo, void* heapStart) {
	// set heap variables
	// heap_start aligned to page directory boundaries (4MB) by linker
	int heapStartPageDirectory = (uintptr_t)heapStart >> 24; //
	int userStartPageDirectory = MEM_USR_START >> 24;
	int usableEndPageDirectory = ((multiboot_meminfo->mem_lower * 1024 + multiboot_meminfo->mem_upper * 1024) >> 24) - 1; // multiboot_meminfo shows memory in counts of KB

	// Init CR3 register
	pageDirectoryCR3.pageWriteThrough = false;
	pageDirectoryCR3.pageCacheDisable = false;
	pageDirectoryCR3.address = (uint32_t)&pageDirectory >> 12;

	//Init page directory & page tables
	for (int pageDirectoryIndex = 0; pageDirectoryIndex < 1024; pageDirectoryIndex++) {
		if (pageDirectoryIndex >= MEM_USR_START >> 24) panic("Too little memory");

		if (pageDirectoryIndex < userStartPageDirectory) {	// Kernel mode
			// Init memory descriptors
			processMemoryDescriptor[pageDirectoryIndex].references = 1;
			processMemoryDescriptor[pageDirectoryIndex].pid = SCHED_PID_KERNEL;
			processMemoryDescriptor[pageDirectoryIndex].pageDirectory = &pageDirectory[pageDirectoryIndex];

			pageDirectory[pageDirectoryIndex].present = true;
			pageDirectory[pageDirectoryIndex].readWrite = true;
			pageDirectory[pageDirectoryIndex].userMode = false;
			pageDirectory[pageDirectoryIndex].pageWriteThrough = false;
			pageDirectory[pageDirectoryIndex].pageCacheDisable = false;
			pageDirectory[pageDirectoryIndex].size = true;
			pageDirectory[pageDirectoryIndex].address = (uint32_t)&pageTable[pageDirectoryIndex] >> 12;
			for (int pageTableIndex = 0; pageTableIndex < 1024; pageTableIndex++) {
				pageTable[pageDirectoryIndex][pageTableIndex].present = true;
				pageTable[pageDirectoryIndex][pageTableIndex].readWrite = true;
				pageTable[pageDirectoryIndex][pageTableIndex].userMode = false;
				pageTable[pageDirectoryIndex][pageTableIndex].pageWriteThrough = false;
				pageTable[pageDirectoryIndex][pageTableIndex].pageCacheDisable = false;
				pageTable[pageDirectoryIndex][pageTableIndex].PAT = false;
				pageTable[pageDirectoryIndex][pageTableIndex].global = false;
				pageTable[pageDirectoryIndex][pageTableIndex].address = (uint32_t)(pageDirectoryIndex * 0x400000 + pageTableIndex * 0x1000) >> 12;
			}
		}
		else {	// User mode heap
			pageDirectory[pageDirectoryIndex].present = false;
		}
	}

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

	// Other misc memory tasks

	// Clear kernel heap area to 0
	for (uintptr_t i = heapStart; i < usableEndPageDirectory << 24; heapStart++) {
		*(uintptr_t*)i = 0;
	}
	
	// Initialize kmalloc variables

	firstNode = (memoryMallocNode_t*)heapStart;
	firstNode->size = sizeof(memoryMallocNode_t);
	firstNode->prev = nullptr;
	firstNode->next = nullptr;


// 	void* kernel_start_page = (void*)(multiboot_meminfo->mem_lower * 1024);
// 	void* heap_start_page = heapStart;
// 	void* heap_end_page = (void*) (multiboot_meminfo->mem_upper * 1024 + (uint32_t)kernel_start_page);
// 	heap_start = heap_start_page;
// 	heap_end = heap_end_page - 1;                                                               // Leave last page free cuz lazy to do all the checking

// 	// Misc stuff before initializing paging
// 	for (uintptr_t i = 0; i < 65536; i++) {
// 		process_memory_descriptor[SCHED_PID_KERNEL].memory_page_count = 0;
// 	}

// 	// Clear multiboot2 info
// 	println("Checking & clearing memory");
// 	for (void* startPage = heap_start_page; (uintptr_t)startPage < (uintptr_t)heap_end_page + 0x1000; startPage = (void*)((uintptr_t)startPage + 0x400000)) {	// Leave several last pages as they contain acpi tables
// 		char tempString[64];
// 		printsn(itoa((uintptr_t)startPage, tempString, 16));
// 		memset(startPage, 0, 0x400000);
// 	}
// 	println("");
// 	//memset(heap_start_page, 0, (uintptr_t)heap_end_page + 0x1000 - (uintptr_t) heap_start_page);    
	// set CR3 register
	// Initialize page descriptor tables
	
}








































// void* malloc(uintptr_t size) {
// 	// Kernel critical section
// 	scheduler_kernel_uninterruptible();

// 	// malloc now respects page boundaries
// 	// All memory allocations are word size aligned for speed;
// 	if (size <= 0xFFFFFFFC) {
// 		if (size % 4) {
// 			size = size & 0xFFFFFFFC;
// 			size += 4;
// 		}
// 	}
// 	else {
// 		println("Memory allocation request too large!");
// 		panic();
// 	}

// 	memory_malloc_node_t* node = heap_start;
// 	memory_malloc_node_t* lastNode = node;
// 	while (1) {
// 		// Check if the page of current node pointer exists (preventative page fault checks)
// 		if (!memory_kernel_check_exists(node)) {
// 			memory_kernel_page_alloc(node);
// 		}

// 		// Check if first empty node
// 		if (node->prevNode == nullptr && node->nextNode == nullptr && node->size == 0) {
// 			// Allocate respective pages and references
// 			// Check how many pages this malloc crosses
// 			for (void* pageIndex = (void*)((uintptr_t)node & 0xFFFFF000); (uintptr_t)pageIndex <= (uintptr_t)node + size + sizeof(memory_malloc_node_t); pageIndex += 0x1000) {
// 				uintptr_t i = ((uintptr_t)pageIndex & 0xFFC00000) >> 22;
// 				uintptr_t j = ((uintptr_t)pageIndex & 0x3FF000) >> 12;
// 				if (!memory_kernel_check_exists(pageIndex)) {
// 					memory_kernel_page_alloc(pageIndex);
// 				}
// 				virtual_page_descriptor[i][j].references++;
// 			}

// 			// Add nodes and allocate new pages if necessary
// 			if ((uintptr_t)node + size + sizeof(memory_malloc_node_t) < ((uintptr_t)node & 0xFFFFF000) + 0x1000) {		// Fits within current page

// 				// Insert new node to end of list
// 				node->size = size + sizeof(memory_malloc_node_t);

// 				break;
// 			}
// 			else {																				// Crosses over page, need to allocate new page
// 				// Insert new node to end of list
// 				node->size = size + sizeof(memory_malloc_node_t);

// 				break;
// 			}
// 		}
// 		// If not first empty node
// 		else {
// 			// Check if last node
// 			if (node->nextNode == nullptr) {
// 				lastNode = node;
// 				node = (memory_malloc_node_t*)((uintptr_t)node + node->size);
// 				// Allocate respective pages and references
// 				// Check how many pages this malloc crosses
// 				for (void* pageIndex = (void*)((uintptr_t)node & 0xFFFFF000); (uintptr_t)pageIndex <= (uintptr_t)node + size + sizeof(memory_malloc_node_t); pageIndex += 0x1000) {
// 					uintptr_t i = ((uintptr_t)pageIndex & 0xFFC00000) >> 22;
// 					uintptr_t j = ((uintptr_t)pageIndex & 0x3FF000) >> 12;
// 					if (!memory_kernel_check_exists(pageIndex)) {
// 						memory_kernel_page_alloc(pageIndex);
// 					}
// 					virtual_page_descriptor[i][j].references++;
// 				}

// 				// Insert new node to end of list
// 				node->size = size + sizeof(memory_malloc_node_t);
// 				node->prevNode = lastNode;
// 				node->nextNode = nullptr;
// 				lastNode->nextNode = node;

// 				break;
// 			}
// 			// If not last node
// 			else if ((uintptr_t)node->nextNode - (uintptr_t)node - node->size >= size + sizeof(memory_malloc_node_t)){		// Increment to next node or check space between nodes
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

// 				// Check how many pages this malloc crosses
// 				// No need to allocate pages as this is in between nodes
// 				for (void* pageIndex = (void*)((uintptr_t)node & 0xFFFFF000); (uintptr_t)pageIndex <= (uintptr_t)node + size + sizeof(memory_malloc_node_t); pageIndex += 0x1000) {
// 					uintptr_t i = ((uintptr_t)pageIndex & 0xFFC00000) >> 22;
// 					uintptr_t j = ((uintptr_t)pageIndex & 0x3FF000) >> 12;
// 					virtual_page_descriptor[i][j].references++;
// 				}

// 				break;
// 			}
// 			else {
// 				lastNode = node;
// 				node = (memory_malloc_node_t*)(node->size + (uintptr_t)node);
// 			}
// 		}
// 	}
	
// 	scheduler_kernel_interruptible();
// 	return &node->data;
// }

// void* malloc_align(uintptr_t size, uintptr_t align) {
//     // Kernel critical section
// 	scheduler_kernel_uninterruptible();

// 	// malloc now respects page boundaries
// 	// All memory allocations are word size aligned for speed;
// 	if (size <= 0xFFFFFFFC) {
// 		if (size % 4) {
// 			size = size & 0xFFFFFFFC;
// 			size += 4;
// 		}
// 	}
// 	else {
// 		println("Memory allocation request too large!");
// 		panic();
// 	}

//     // Check align parameter
    

// 	memory_malloc_node_t* node = heap_start;
// 	memory_malloc_node_t* lastNode = node;
// 	while (1) {
// 		// Check if the page of current node pointer exists (preventative page fault checks)
// 		if (!memory_kernel_check_exists(node)) {
// 			memory_kernel_page_alloc(node);
// 		}

// 		// Check if first empty node
// 		if (node->prevNode == nullptr && node->nextNode == nullptr && node->size == 0) {
//             // Align node
//             if ((uintptr_t)node % align) {
//                 uintptr_t offset = (uintptr_t)node % align;
//                 node = (memory_malloc_node_t*)((uintptr_t)node - offset);
//                 node = (memory_malloc_node_t*)((uintptr_t)node + align);
//                 //node = (memory_malloc_node_t*)((uintptr_t)node - (uintptr_t)node % align + align);
//             }

// 			// Allocate respective pages and references
// 			// Check how many pages this malloc crosses
// 			for (void* pageIndex = (void*)((uintptr_t)node & 0xFFFFF000); (uintptr_t)pageIndex <= (uintptr_t)node + size + sizeof(memory_malloc_node_t) + ((uintptr_t)pageIndex); pageIndex += 0x1000) {
// 				uintptr_t i = ((uintptr_t)pageIndex & 0xFFC00000) >> 22;
// 				uintptr_t j = ((uintptr_t)pageIndex & 0x3FF000) >> 12;
// 				if (!memory_kernel_check_exists(pageIndex)) {
// 					memory_kernel_page_alloc(pageIndex);
// 				}
// 				virtual_page_descriptor[i][j].references++;
// 			}

// 			// Add nodes and allocate new pages if necessary
// 			if ((uintptr_t)node + size + sizeof(memory_malloc_node_t) < ((uintptr_t)node & 0xFFFFF000) + 0x1000) {		// Fits within current page

// 				// Insert new node to end of list
// 				node->size = size + sizeof(memory_malloc_node_t);

// 				break;
// 			}
// 			else {																				// Crosses over page, need to allocate new page
// 				// Insert new node to end of list
// 				node->size = size + sizeof(memory_malloc_node_t);

// 				break;
// 			}
// 		}
// 		// If not first empty node
// 		else {
// 			// Check if last node
// 			if (node->nextNode == nullptr) {
// 				lastNode = node;
// 				node = (memory_malloc_node_t*)((uintptr_t)node + node->size);

//                 // Align node
//                 if ((uintptr_t)node % align) {
//                     uintptr_t offset = (uintptr_t)node % align;
//                     node = (memory_malloc_node_t*)((uintptr_t)node - offset);
//                     node = (memory_malloc_node_t*)((uintptr_t)node + align);
//                 }

// 				// Allocate respective pages and references
// 				// Check how many pages this malloc crosses
// 				for (void* pageIndex = (void*)((uintptr_t)node & 0xFFFFF000); (uintptr_t)pageIndex <= (uintptr_t)node + size + sizeof(memory_malloc_node_t); pageIndex += 0x1000) {
// 					uintptr_t i = ((uintptr_t)pageIndex & 0xFFC00000) >> 22;
// 					uintptr_t j = ((uintptr_t)pageIndex & 0x3FF000) >> 12;
// 					if (!memory_kernel_check_exists(pageIndex)) {
// 						memory_kernel_page_alloc(pageIndex);
// 					}
// 					virtual_page_descriptor[i][j].references++;
// 				}

// 				// Insert new node to end of list
// 				node->size = size + sizeof(memory_malloc_node_t);
// 				node->prevNode = lastNode;
// 				node->nextNode = nullptr;
// 				lastNode->nextNode = node;

// 				break;
// 			}
// 			// If not last node
// 			else if ((uintptr_t)node->nextNode - (uintptr_t)node - node->size >= size + sizeof(memory_malloc_node_t) + (align - (uintptr_t)node % align)){		// Increment to next node or check space between nodes

//                 // Align node
//                 if ((uintptr_t)node % align) {
//                     uintptr_t offset = (uintptr_t)node % align;
//                     node = (memory_malloc_node_t*)((uintptr_t)node - offset);
//                     node = (memory_malloc_node_t*)((uintptr_t)node + align);
//                 }
                
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

// 				// Check how many pages this malloc crosses
// 				// No need to allocate pages as this is in between nodes
// 				for (void* pageIndex = (void*)((uintptr_t)node & 0xFFFFF000); (uintptr_t)pageIndex <= (uintptr_t)node + size + sizeof(memory_malloc_node_t); pageIndex += 0x1000) {
// 					uintptr_t i = ((uintptr_t)pageIndex & 0xFFC00000) >> 22;
// 					uintptr_t j = ((uintptr_t)pageIndex & 0x3FF000) >> 12;
// 					virtual_page_descriptor[i][j].references++;
// 				}

// 				break;
// 			}
// 			else {
// 				lastNode = node;
// 				node = (memory_malloc_node_t*)(node->size + (uintptr_t)node);
// 			}
// 		}
// 	}
	
// 	scheduler_kernel_interruptible();
// 	return &node->data;
// }

// void memory_direct_map(void* virtualAddress, void* physicalAddress) {
// 	// Kernel critical section
// 	scheduler_kernel_uninterruptible();
	
// 	uintptr_t i = ((uintptr_t)virtualAddress & 0xFFC00000) >> 22;
// 	uintptr_t j = ((uintptr_t)virtualAddress & 0x3FF000) >> 12;

// 	uintptr_t x = ((uintptr_t)physicalAddress & 0xFFC00000) >> 22;
// 	uintptr_t y = ((uintptr_t)physicalAddress & 0x3FF000) >> 12;

// 	if (virtual_page_descriptor[i][j].pid != SCHED_PID_NOTHING || physical_page_descriptor[x][y].virtual_page_descriptor != nullptr) {
// 		printf("Error malloc_direct_map(%x, %x), page already allocated\n", (uintptr_t)virtualAddress, (uintptr_t)physicalAddress);
// 		panic();
// 	}
// 	else {
// 		// Virtual page descriptor stuff
// 		virtual_page_descriptor[i][j].pid = SCHED_PID_KERNEL;
// 		virtual_page_descriptor[i][j].references = 0;
// 		virtual_page_descriptor[i][j].pageTable = &pageTable[i][j];
// 		virtual_page_descriptor[i][j].pageTable->present = 1;
// 		virtual_page_descriptor[i][j].pageTable->RW = 1;
// 		virtual_page_descriptor[i][j].pageTable->US = 0;
// 		virtual_page_descriptor[i][j].pageTable->pageWriteThrough = 0;
// 		virtual_page_descriptor[i][j].pageTable->pageCacheDisable = 0;
// 		virtual_page_descriptor[i][j].pageTable->global = 0;
// 		virtual_page_descriptor[i][j].pageTable->address = (x * 1024 * 4096 + y * 4096) >> 12;
// 		virtual_page_descriptor[i][j].physical_page_descriptor = &physical_page_descriptor[x][y];

// 		// Physical page descriptor stuff
// 		physical_page_descriptor[x][y].virtual_page_descriptor = &virtual_page_descriptor[i][j];
// 	}

// 	scheduler_kernel_interruptible();
// }

// void free(void* address) {
// 	// Kernel critical section
// 	scheduler_kernel_uninterruptible();

// 	memory_malloc_node_t* node = (memory_malloc_node_t*)((uintptr_t)address - sizeof(memory_malloc_node_t));

// 	uintptr_t size = node->size;
	
// 	// Clear data
// 	memset(&node->data, 0, size);

// 	// Connect prev node and next node
// 	memory_malloc_node_t* prevNode = node->prevNode;
// 	memory_malloc_node_t* nextNode = node->nextNode;
// 	prevNode->nextNode = nextNode;
// 	if (nextNode != nullptr) {
// 		nextNode->prevNode = prevNode;
// 	}

// 	node->size = 0;
// 	node->nextNode = (memory_malloc_node_t*)nullptr;
// 	node->prevNode = (memory_malloc_node_t*)nullptr;

// 	// Remove references from pages
// 	for (void* pageIndex = (void*)((uintptr_t)node & 0xFFFFF000); (uintptr_t)pageIndex <= (uintptr_t)node + size; pageIndex += 0x1000) {
// 		uintptr_t i = ((uintptr_t)pageIndex & 0xFFC00000) >> 22;
// 		uintptr_t j = ((uintptr_t)pageIndex & 0x3FF000) >> 12;
// 		virtual_page_descriptor[i][j].references--;
// 		if (virtual_page_descriptor[i][j].references == 0) {
// 			memory_kernel_page_free(pageIndex);
// 		}
// 	}

// 	scheduler_kernel_interruptible();
// }

// void memory_interrupt_handler(IDT_pageFault_error_t pageFault_error, void* address, void* instruction) {
// 	char tempString[64];
// 	if (pageFault_error.U == 0) {
// 		if (pageFault_error.P == 0)	{
// 			if (pageFault_error.W == 0) {
// 				println("Kernel panic! Invalid kernel memory read access");
// 				println("Fault occured at");
// 				printf("Instruction: ");
// 				println(itoa((uintptr_t)instruction, tempString, 16));
// 				printf("Address: ");
// 				println(itoa((uintptr_t)address, tempString, 16));
// 			}
// 			else {
// 				println("Kernel panic! Invalid kernel memory write access");
// 				println("Fault occured at");
// 				printf("Instruction: ");
// 				println(itoa((uintptr_t)instruction, tempString, 16));
// 				printf("Address: ");
// 				println(itoa((uintptr_t)address, tempString, 16));
// 			}

// 		}
// 		panic();
// 	}
// 	else {

// 	}
// }

// void memory_kernel_page_alloc(void* address) {
// 	// Kernel critical section
// 	scheduler_kernel_uninterruptible();
	
// 	// Check if kernel page is already allocated
// 	uintptr_t i = ((uintptr_t)address & 0xFFC00000) >> 22;
// 	uintptr_t j = ((uintptr_t)address & 0x3FF000) >> 12;
// 	// Find free page
// 	for (uintptr_t index = 0; index < 1024 * 1024; index++) {
// 		uintptr_t x = index / 1024;
// 		uintptr_t y = index % 1024;

// 		if (physical_page_descriptor[x][y].virtual_page_descriptor == nullptr) {
// 			// Virtual page descriptor stuff
// 			virtual_page_descriptor[i][j].pid = SCHED_PID_KERNEL;
// 			virtual_page_descriptor[i][j].references = 0;
// 			virtual_page_descriptor[i][j].pageTable = &pageTable[i][j];
// 			virtual_page_descriptor[i][j].pageTable->present = 1;
// 			virtual_page_descriptor[i][j].pageTable->RW = 1;
// 			virtual_page_descriptor[i][j].pageTable->US = 0;
// 			virtual_page_descriptor[i][j].pageTable->pageWriteThrough = 0;
// 			virtual_page_descriptor[i][j].pageTable->pageCacheDisable = 0;
// 			virtual_page_descriptor[i][j].pageTable->global = 0;
// 			virtual_page_descriptor[i][j].pageTable->address = (x * 1024 * 4096 + y * 4096) >> 12;
// 			virtual_page_descriptor[i][j].physical_page_descriptor = &physical_page_descriptor[x][y];

// 			// Physical page descriptor stuff
// 			physical_page_descriptor[x][y].virtual_page_descriptor = &virtual_page_descriptor[i][j];

// 			break;
// 		}
// 	}

// 	scheduler_kernel_interruptible();
// }

// void memory_kernel_page_free(void* address) {
// 	// Kernel critical section
// 	scheduler_kernel_uninterruptible();

// 	uintptr_t i = ((uintptr_t)address & 0xFFC00000) >> 22;
// 	uintptr_t j = ((uintptr_t)address & 0x3FF000) >> 12;

// 	// Check if already free
// 	if (virtual_page_descriptor[i][j].pid == SCHED_PID_NOTHING) {
// 		printf("Error memory_kernel_page_free(%x), page already allocated\n", (uintptr_t)address);
// 		panic();
// 	}
// 	else {
// 		// Virtual page descriptor stuff
// 		virtual_page_descriptor[i][j].pid = SCHED_PID_NOTHING;
// 		virtual_page_descriptor[i][j].references = 0;
// 		virtual_page_descriptor[i][j].pageTable->present = 0;

// 		// Physical page descriptor stuff
// 		virtual_page_descriptor[i][j].physical_page_descriptor->virtual_page_descriptor = nullptr;
// 		virtual_page_descriptor[i][j].physical_page_descriptor = nullptr;
// 	}
	
// 	scheduler_kernel_interruptible();
// }

// bool memory_kernel_check_exists(void* address) {
// 	// Kernel critical section due to race conditions
// 	scheduler_kernel_uninterruptible();
	
// 	bool ret;
	
// 	uintptr_t i = ((uintptr_t)address & 0xFFC00000) >> 22;
// 	uintptr_t j = ((uintptr_t)address & 0x3FF000) >> 12;
// 	if (virtual_page_descriptor[i][j].pid == SCHED_PID_NOTHING)
// 	{
// 		ret = false;
// 	}
// 	else {
// 		ret = true;
// 	}

// 	scheduler_kernel_interruptible();

// 	return ret;
// }

// bool memory_init(struct multiboot_tag_basic_meminfo* multiboot_meminfo, void* heapStart) {
// /*-----------------------------------------------------------------------------------------------*/
// /*                                          Memory Type                                          */
// /*-----------------------------------------------------------------------------------------------*/

// 	void* kernel_start_page = (void*)(multiboot_meminfo->mem_lower * 1024);
// 	void* heap_start_page = heapStart;
// 	void* heap_end_page = (void*) (multiboot_meminfo->mem_upper * 1024 + (uint32_t)kernel_start_page);
// 	heap_start = heap_start_page;
// 	heap_end = heap_end_page - 1;                                                               // Leave last page free cuz lazy to do all the checking

// 	// Misc stuff before initializing paging
// 	for (uintptr_t i = 0; i < 65536; i++) {
// 		process_memory_descriptor[SCHED_PID_KERNEL].memory_page_count = 0;
// 	}

// 	// Clear multiboot2 info
// 	println("Checking & clearing memory");
// 	for (void* startPage = heap_start_page; (uintptr_t)startPage < (uintptr_t)heap_end_page + 0x1000; startPage = (void*)((uintptr_t)startPage + 0x400000)) {	// Leave several last pages as they contain acpi tables
// 		char tempString[64];
// 		printsn(itoa((uintptr_t)startPage, tempString, 16));
// 		memset(startPage, 0, 0x400000);
// 	}
// 	println("");
// 	//memset(heap_start_page, 0, (uintptr_t)heap_end_page + 0x1000 - (uintptr_t) heap_start_page);       

// /*-----------------------------------------------------------------------------------------------*/
// /*                                          Paging Init                                          */
// /*-----------------------------------------------------------------------------------------------*/

// 	// Initialization of paging structures
// 	// Only enable kernel space memory for now, enable MMIO and user space later in init process

// 	// Init pageDirectoryCR3
// 	pageDirectoryCR3.address = (uint32_t)&pageDirectory >> 12;
// 	pageDirectoryCR3.pageWriteThrough = 0;
// 	pageDirectoryCR3.pageCacheDisable = 0;

// 	// Init pageDirectory
// 	{
// 		void* currentPageIndex;
// 		void* heapStartPageDirectory;
// 		uintptr_t temp = (uintptr_t)heap_start_page;
// 		if (temp & 0x400000) {
// 			temp &= 0xFF400000;
// 			temp += 0x400000;
// 		}
// 		heapStartPageDirectory = (void*) temp;
// 		for (uintptr_t i = 0; i < 1024; i++) {
// 			currentPageIndex = (void*)(i * 1024 * 4096);
// 			if (currentPageIndex <= heapStartPageDirectory) {                                   // Kernel space
// 				currentPageIndex = (void*)(i * 1024 * 4096);
// 				pageDirectory[i].present = 1;
// 				pageDirectory[i].RW = 1;
// 				pageDirectory[i].US = 0;
// 				pageDirectory[i].pageWriteThrough = 0;
// 				pageDirectory[i].pageCacheDisable = 0;
// 				pageDirectory[i].size = 1;
// 				pageDirectory[i].address = (uint32_t)&pageTable[i] >> 12;
// 				for (uintptr_t j = 0; j < 1024; j++) {
// 					currentPageIndex = (void*)(i * 1024 * 4096 + j * 4096);
// 					if (currentPageIndex < heap_start_page) {
// 						pageTable[i][j].present = 1;
// 						pageTable[i][j].RW = 1;
// 						pageTable[i][j].US = 0;
// 						pageTable[i][j].pageWriteThrough = 0;
// 						pageTable[i][j].pageCacheDisable = 0;
// 						pageTable[i][j].global = 0;													// Will set to global cuz lazy to implement TLBs
// 						pageTable[i][j].address = ((uint32_t)currentPageIndex) >> 12;

// 						// Virtual page descriptor stuff
// 						virtual_page_descriptor[i][j].pid = SCHED_PID_KERNEL;
// 						virtual_page_descriptor[i][j].pageTable = &pageTable[i][j];
// 						virtual_page_descriptor[i][j].references = 0xFFFFFFFF;
// 						virtual_page_descriptor[i][j].physical_page_descriptor = &physical_page_descriptor[i][j];

// 						// Physical page descriptor stuff
// 						physical_page_descriptor[i][j].virtual_page_descriptor = &virtual_page_descriptor[i][j];

// 						// Process descriptor stuff
// 						process_memory_descriptor[SCHED_PID_KERNEL].memory_page_count++;
// 					}
// 					else {
// 						pageTable[i][j].present = 0;

// 						// Virtual page descriptor stuff
// 						virtual_page_descriptor[i][j].pid = SCHED_PID_NOTHING;
// 						virtual_page_descriptor[i][j].pageTable = &pageTable[i][j];

// 						// Physical page descriptor stuff
// 						physical_page_descriptor[i][j].virtual_page_descriptor = nullptr;
// 					}
// 				}
// 			}
// 			else {
// 				pageDirectory[i].present = 1;
// 				pageDirectory[i].RW = 1;
// 				pageDirectory[i].US = 0;
// 				pageDirectory[i].pageWriteThrough = 0;
// 				pageDirectory[i].pageCacheDisable = 0;
// 				pageDirectory[i].size = 1;
// 				pageDirectory[i].address = (uint32_t)&pageTable[i] >> 12;

// 				for (uintptr_t j = 0; j < 1024; j++) {
// 					// Virtual page descriptor stuff
// 					virtual_page_descriptor[i][j].pid = SCHED_PID_NOTHING;
// 					virtual_page_descriptor[i][j].pageTable = &pageTable[i][j];

// 					// Physical page descriptor stuff
// 					physical_page_descriptor[i][j].virtual_page_descriptor = nullptr;
// 				}
// 			}
// 		}
// 	}


// /*-----------------------------------------------------------------------------------------------*/
// /*                                         Enable Paging                                         */
// /*-----------------------------------------------------------------------------------------------*/

// // Move page directory structure to CR3
// // Enable paging
// 	asm volatile (
// 		"mov %0, %%eax              \n\t"
// 		"mov %%eax, %%cr3"

// 		: 
// 		: "r" (pageDirectoryCR3)
// 	);
// 	asm volatile (
// 		"mov %cr0, %eax           \n\t"
// 		"or $0x80000001, %eax      \n\t"
// 		"mov %eax, %cr0"
// 	);

// /*-----------------------------------------------------------------------------------------------*/
// /*                                     Misc memory functions                                     */
// /*-----------------------------------------------------------------------------------------------*/

// 	// malloc init, clear multiboot2 information and other data       
// 	uintptr_t* tempFix = kmalloc(4);																// TEMPFIX: Because of the first node problem of malloc, need an unreferenced first node to keep the references for the second node
// 	(*tempFix) = 0x12345678;



// 	return false;
// }
