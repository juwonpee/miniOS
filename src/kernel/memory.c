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

void memset_char(void* address, char value, uintptr_t size) {
	char* dest_char = (char*)address;
	for (uintptr_t index = 0; index < size; index++) {
		dest_char[index] = value;
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
	if (size == 0) return nullptr;

	// Align size too 4 bytes
	if (size % 4) {
		size = size & 0xFFFFFFFC;
		size += 4;
	}

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
		if (node->next != nullptr) { // If not last node
			// Check space between current and next node
			if ((intptr_t)((uintptr_t)node->next - (uintptr_t)node - node->size - size - sizeof(memoryMallocNode_t)) >= 0) {	// If space is big enough
				// assign space inbetween node
				prevNode = node;
				nextNode = node->next;
				node = (memoryMallocNode_t*)((uintptr_t)node + node->size);
				break;
			}
			node = node->next;
		}
		else {	// If last node
			prevNode = node;
			nextNode = nullptr;
			node = (memoryMallocNode_t*)((uintptr_t)node + node->size);
			break;
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

	// Zero out data section
	uintptr_t* data = (uintptr_t*)&node->data;
	for (uintptr_t index = 0; index < (node->size - sizeof(memoryMallocNode_t))/4; index++) {
		data[index] = 0;
	}

	return &node->data;
}

void* kmalloc_align(uintptr_t size, uintptr_t align) {
	if (size == 0) return nullptr;

	// Align size too 4 bytes
	if (size % sizeof(uintptr_t)) {
		size = size & 0xFFFFFFFC;
		size += sizeof(uintptr_t);
	}

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
		if (node->next != nullptr) { // If not last node
			// Check space between current and next node
			
			if ((intptr_t)((uintptr_t)node->next - (uintptr_t)node - node->size - size - sizeof(memoryMallocNode_t)) >= 0) {	// If space is big enough
				// assign space inbetween node
				prevNode = node;
				nextNode = node->next;
				node = (memoryMallocNode_t*)((uintptr_t)node + node->size);
				break;
			}
			node = node->next;
		}
		else {	// If last node
			prevNode = node;
			nextNode = nullptr;
			node = (memoryMallocNode_t*)((uintptr_t)node + node->size);
			break;
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

	// Zero out data section
	uintptr_t* data = (uintptr_t*)&node->data;
	for (uintptr_t index = 0; index < (node->size - sizeof(memoryMallocNode_t))/4; index++) {
		data[index] = 0;
	}

	return &node->data;
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

void memory_interrupt_handler(IDT_pageFault_error_t pageFault_error, void* address, void* instruction) {
	char tempString[64];
	if (pageFault_error.U == 0) {
		if (pageFault_error.P == 0)	{
			if (pageFault_error.W == 0) {
				println("Kernel panic! Invalid kernel memory read access");
				println("Fault occured at");
				printf("Instruction: ");
				println(itoa((uintptr_t)instruction, tempString, 16));
				printf("Address: ");
				println(itoa((uintptr_t)address, tempString, 16));
			}
			else {
				println("Kernel panic! Invalid kernel memory write access");
				println("Fault occured at");
				printf("Instruction: ");
				println(itoa((uintptr_t)instruction, tempString, 16));
				printf("Address: ");
				println(itoa((uintptr_t)address, tempString, 16));
			}

		}
		panic("");
	}
}

bool memory_init(struct multiboot_tag_basic_meminfo* multiboot_meminfo, void* heapStart) {


	// set heap variables
	// heap_start aligned to page directory boundaries (4MB) by linker
	int heapStartPageDirectory = (uintptr_t)heapStart >> 22; //
	int userStartPageDirectory = MEM_USR_START >> 22;
	int usableEndPageDirectory = ((multiboot_meminfo->mem_lower * 1024 + multiboot_meminfo->mem_upper * 1024) >> 24) - 1; // multiboot_meminfo shows memory in counts of KB
	
	// Check memory size
	if (userStartPageDirectory > usableEndPageDirectory) panic("Too little memory");

	// Init CR3 register
	pageDirectoryCR3.pageWriteThrough = false;
	pageDirectoryCR3.pageCacheDisable = false;
	pageDirectoryCR3.address = (uint32_t)&pageDirectory >> 12;

	//Init page directory & page tables
	for (int pageDirectoryIndex = 0; pageDirectoryIndex < 1024; pageDirectoryIndex++) {

		if (pageDirectoryIndex < userStartPageDirectory) {	// Kernel mode
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
	for (uintptr_t* i = (uintptr_t*)(heapStartPageDirectory << 22); (uintptr_t)i < userStartPageDirectory << 22; i++) {
		*i = 0;
	}
	
	// Initialize kmalloc variables

	firstNode = (memoryMallocNode_t*)heapStart;
	firstNode->size = sizeof(memoryMallocNode_t);
	firstNode->prev = firstNode;
	firstNode->next = nullptr;


	return false;
}





