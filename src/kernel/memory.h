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

#pragma once

#include <stdalign.h>
#include "types.h"
#include "multiboot2.h"
#include "print.h"
#include "string.h"
#include "panic.h"
#include "sched.h"
#include "interrupt.h"

#define nullptr (void*)0x0

typedef struct memory_pageDirectoryCR3_t {
	union {
		uint32_t data;
		struct {
			uint32_t ignore1:3;
			uint32_t pageWriteThrough:1;
			uint32_t pageCacheDisable:1;
			uint32_t ignore2:7;
			uint32_t address:20;
		};
	};
} __attribute__ ((packed)) memory_pageDirectoryCR3_t;

typedef struct memory_pageDirectory_t {
	union {
		uint32_t data;
		struct {
			uint32_t present:1;
			uint32_t RW:1;                                          // Read/Write
			uint32_t US:1;                                          // 0: Supervisor						1: User
			uint32_t pageWriteThrough:1;                            // 0: Write back						1: Write through
			uint32_t pageCacheDisable:1;							// 0: Cache								1: Cache disable
			uint32_t accessed:1;
			uint32_t ignore1:1;
			uint32_t size:1;                                        // 0: 4MB pages, 1: 4KiB pages
			uint32_t ignore2:4;
			uint32_t address:20;
		};
	};
} __attribute__ ((packed)) memory_pageDirectory_t;

typedef struct memory_pageTable_t {
	union {
		uint32_t data;
		struct {
			uint32_t present:1;
			uint32_t RW:1;                                         // Read/Write
			uint32_t US:1;                                          // 0: Supervisor						1: User
			uint32_t pageWriteThrough:1;                            // 0: Write back						1: Write through
			uint32_t pageCacheDisable:1;							// 0: Cache								1: Cache disable
			uint32_t accessed:1;
			uint32_t dirty:1;
			uint32_t PAT:1;                                         // PAT, not enabled so must stay zero
			uint32_t global:1;                                      // 0: Invalidate TLB on CR3 flush		1: Do not invalidate
			uint32_t ignore:3;
			uint32_t address:20;
		};
	};
} __attribute__ ((packed)) memory_pageTable_t;

typedef struct memory_pageDirectory_owner_t {
	memory_pageDirectory_t* page;
	bool free;
	uintptr_t pid;
} memory_pageDirectory_owner_t;

typedef struct memory_malloc_node_t {
	uintptr_t size;																				// Size for the total structure
	struct memory_malloc_node_t* prevNode;												// Included prev node pointer to effeciently finding the previous node in free()
	struct memory_malloc_node_t* nextNode;												// Included next node pointer to effeciently find the next node or free space
	uint8_t data[];
} memory_malloc_node_t;

typedef struct memory_virtual_page_descriptor_t memory_virtual_page_descriptor_t;
typedef struct memory_physical_page_descriptor_t memory_physical_page_descriptor_t;

typedef struct memory_virtual_page_descriptor_t {
	uintptr_t references;
	scheduler_pid_t pid;
	memory_pageTable_t* pageTable;
	memory_physical_page_descriptor_t* physical_page_descriptor;
} memory_virtual_page_descriptor_t;

typedef struct memory_physical_page_descriptor_t {
	memory_virtual_page_descriptor_t* virtual_page_descriptor;
} memory_physical_page_descriptor_t;

typedef struct memory_process_memory_descriptor_t {
	uintptr_t memory_page_count;
} memory_process_memory_descriptor_t;



void memset(void* address, char value, uintptr_t size);

void memcpy(void* dest, void* src, uintptr_t size);

void* malloc(uintptr_t size);

// align parameter is a bit mask for the align size, 0x10: align at 16 byte boundaries, 0x1000: align at 4096 byte boundaries
void* malloc_align(uintptr_t size, uintptr_t align);

void memory_direct_map(void* virtualAddress, void* physicalAddress);

void free(void* address);

void memory_kernel_page_alloc(void* address);

void memory_kernel_page_free(void*  address);

void memory_interrupt_handler(IDT_pageFault_error_t pageFault_error, void* address, void* instruction);

bool memory_kernel_check_exists(void* address);

bool memory_init(struct multiboot_tag_basic_meminfo* multiboot_meminfo, void* heapStart);