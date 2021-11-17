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

void memset(void* address, char value, uintptr_t size);

void memcpy(void* dest, void* src, uintptr_t size);

void* malloc(uintptr_t size);

void malloc_direct_map(void* virtualAddress, void* physicalAddress);

void free(void* address);

void memory_interrupt_handler(IDT_pageFault_error_t pageFault_error, void* address, void* instruction);

void memory_kernel_page_alloc(void* address);

void memory_kernel_page_free(void* address);

bool memory_kernel_check_exists(void* address);

bool memory_init(struct multiboot_tag_basic_meminfo* multiboot_meminfo, void* heapStart);