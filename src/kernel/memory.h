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
#include "multiboot.h"
#include "driver/print.h"
#include "string.h"

#define UHEAP_START 0x4000000   // from 64MB for userland code heap paging
#define nullptr (void*)0x0

typedef struct pageDirectoryCR3_t {
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
} __attribute__ ((packed)) pageDirectoryCR3_t;

typedef struct pageDirectory_t {
    union {
        uint32_t data;
        struct {
            uint32_t present:1;
            uint32_t RW:1;                                          // Read/Write
            uint32_t US:1;                                          // 0: Supervisor, 1: User
            uint32_t pageWriteThrough:1;
            uint32_t pageCacheDisable:1;
            uint32_t accessed:1;
            uint32_t ignore1:1;
            uint32_t size:1;                                        // 0: 4MB pages, 1: 4KiB pages
            uint32_t ignore2:4;
            uint32_t address:20;
        };
    };
} __attribute__ ((packed)) pageDirectory_t;

typedef struct pageTable_t {
    union {
        uint32_t data;
        struct {
            uint32_t present:1;
            uint32_t RW:1;                                         // Read/Write
            uint32_t US:1;                                         // User/Supervisor
            uint32_t pageWriteThrough:1;
            uint32_t pageCacheDisable:1;
            uint32_t accessed:1;
            uint32_t dirty:1;
            uint32_t PAT:1;                                         // PAT something i dont know, must stay 0
            uint32_t global:1;                                      // If set prevent TLB from updating
            uint32_t ignore:3;
            uint32_t address:20;
        };
    };
} __attribute__ ((packed)) pageTable_t;





void* malloc(uint32_t size);
void free(void* address);
bool memory_init(multiboot_info_t* mbd, uint32_t magic, void* heapStart);