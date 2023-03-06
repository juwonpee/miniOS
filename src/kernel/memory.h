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

#include "stdalign.h"
#include "stdint-gcc.h"

#include "multiboot2.h"
#include "print.h"
#include "string.h"
#include "panic.h"
#include "sched.h"
#include "interrupt.h"

#define nullptr (void*)0x0
#define NULL 0

// Memory allocation definitions
#define MEM_KERNEL_START	0x00000000
#define MEM_USR_START 		0x08000000 // 128MB
#define MEM_USR_END 		0xFFFFFFFF

#define MEM_KERNEL_SIZE		MEM_USR_START-MEM_KERNEL_START


typedef struct memoryPageDirectoryCR3_t {
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
} __attribute__ ((packed)) memoryPageDirectoryCR3_t;

typedef struct memoryPageDirectory_t {
	union {
		uint32_t data;
		struct {
			uint32_t present:1;
			uint32_t readWrite:1;                                          // Read/Write
			uint32_t userMode:1;                                          // 0: Supervisor						1: User
			uint32_t pageWriteThrough:1;                            // 0: Write back						1: Write through
			uint32_t pageCacheDisable:1;							// 0: Cache								1: Cache disable
			uint32_t accessed:1;
			uint32_t ignore1:1;
			uint32_t size:1;                                        // 0: 4MB pages, 1: 4KiB pages
			uint32_t ignore2:4;
			uint32_t address:20;
		};
	};
} __attribute__ ((packed)) memoryPageDirectory_t;

typedef struct memoryPageTable_t {
	union {
		uint32_t data;
		struct {
			uint32_t present:1;
			uint32_t readWrite:1;											// Read/Write
			uint32_t userMode:1;											// 0: Supervisor						1: User
			uint32_t pageWriteThrough:1;									// 0: Write back						1: Write through
			uint32_t pageCacheDisable:1;									// 0: Cache								1: Cache disable
			uint32_t accessed:1;
			uint32_t dirty:1;
			uint32_t PAT:1;                                         // PAT, not enabled so must stay zero
			uint32_t global:1;                                      // 0: Invalidate TLB on CR3 flush		1: Do not invalidate
			uint32_t ignore:3;
			uint32_t address:20;
		};
	};
} __attribute__ ((packed)) memoryPageTable_t;

typedef struct memoryMallocNode_t {
	uintptr_t size;															// Size includes the node
	struct memoryMallocNode_t* prev;
	struct memoryMallocNode_t* next;
	uintptr_t data[];
} memoryMallocNode_t;

// INTERRUPT TYPES
typedef struct IDT_pageFault_error_t {
	union {
		uint32_t data;
		struct {
			uint32_t P:1;																		// Present				When set, the page fault was caused by a page-protection violation. When not set, it was caused by a non-present page.
			uint32_t W:1;																		// Write				When set, the page fault was caused by a write access. When not set, it was caused by a read access.
			uint32_t U:1;																		// User					When set, the page fault was caused while CPL = 3. This does not necessarily mean that the page fault was a privilege violation.
			uint32_t R:1;																		// Reserved write		When set, one or more page directory entries contain reserved bits which are set to 1. This only applies when the PSE or PAE flags in CR4 are set to 1.
			uint32_t reserved:28;
		};
	};
} __attribute__ ((packed)) IDT_pageFault_error_t;


// Set range of 
void memset_char(void* address, char value, uintptr_t size);

void memcpy(void* dest, void* src, uintptr_t size);

void* kmalloc(uintptr_t size);

void kfree(void* address);

// align parameter is a bit mask for the align size, 0x10: align at 16 byte boundaries, 0x1000: align at 4096 byte boundaries
void* kmalloc_align(uintptr_t size, uintptr_t align);

void memory_interrupt_handler(IDT_pageFault_error_t pageFault_error, void* address, void* instruction);

bool memory_init(struct multiboot_tag_basic_meminfo* multiboot_meminfo, void* heapStart);