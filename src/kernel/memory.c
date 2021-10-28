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


uintptr_t* kstart;
static uintptr_t* ustart = (void*) UHEAP_START;

uintptr_t* usable_first_address;
uintptr_t* usable_last_address;

volatile memory_pageDirectoryCR3_t pageDirectoryCR3;
alignas(4096) volatile memory_pageDirectory_t pageDirectory[1024];
alignas(4096) volatile memory_pageTable_t pageTable[1024][1024];


void* malloc(uintptr_t size) {
    // All memory allocations are word size aligned
    if (size <= 0xFFFFFFFC) {
        if (size % 0x4) {
            size = size & 0xFFFFFFFC;
            size += 4;
        }
    }
    else {
        println("Memory alloc request too large");
        panic();
    }

    uintptr_t* index = usable_first_address + 4096;
    uintptr_t* data;

    while (1) {
        data = index + 1;
        if ((*index) == 0) {                                                                        // Block is empty
            // Check if block is big enough
            bool fit = true;
            for (uintptr_t* blockIndex = data; (*data) < (*data) + size; blockIndex++) {
                if ((*blockIndex) != 0) {
                    fit = false;
                    index = (uintptr_t*)((uintptr_t)index + (*index));
                    break;
                }
            }
            // If block big enough
            if (fit == true) {
                (*index) = size;
                // zero block
                for (uintptr_t* blockIndex = data; blockIndex < data + size; blockIndex++) {
                    if (*blockIndex) {
                        (*blockIndex) = 0;
                    }
                }
                return (void*) data;
            }
        }
        else {
            index = (uintptr_t*)((uintptr_t)index + (*index) + sizeof(uintptr_t));
        }
        index++;
    }
}

void free(void* address) {
    uintptr_t* index = address - sizeof(uintptr_t);
    uintptr_t* data = address;
    uintptr_t size = (*index);
    (*index) = 0;
    for (uintptr_t* blockIndex = data; (*data) < (*data) + size; blockIndex++) {
        (*blockIndex) = 0;
    }
}

void memset(void* address, char value, uintptr_t size) {
    char* destIndex_char = (char*)address;
    for (uintptr_t index = 0; index < size; index++) {
        destIndex_char[index] = value;        
    }
}

void memcpy(void* dest, void* src, uintptr_t size) {

    char* srcIndex_char = (char*)src;
    char* destIndex_char = (char*)dest;
    uintptr_t* srcIndex_uintptr = (uintptr_t*)src;
    uintptr_t* destIndex_uintptr = (uintptr_t*)dest;

    if (size < 0xFFFF) {
        for (uintptr_t index = 0; index < size; index++) {
            destIndex_char[index] = srcIndex_char[index];
        }
    }
    else {
        uintptr_t size_uintptr = (size & 0xFFFFFFFC) / 4;
        uint8_t remainder = size % 4;

        for (uintptr_t index = 0; index < size_uintptr; index++) {
            destIndex_uintptr[index] = srcIndex_uintptr[index];
        }
        for (uintptr_t index = 0; index < remainder; index++) {
            destIndex_char[index] = srcIndex_char[index];
        }
    }
}

bool memory_init(struct multiboot_tag_basic_meminfo* multiboot_meminfo, void* heapStart) {
    // TODO: BUG HACK
    // linker rounds down _end to the nearest 4096 byte, so there is still data at heapStart
    // skipping to another page to get clear memory
    heapStart += 4096;
    kstart = heapStart;

    // temp string of size 64
    char tempString[64];
    // Variables to initialize memory;
    void* usableFirstPageAddress;
    void* usableLastPageAddress;
    void* finalPageAddress = (void*)0xFFFFF000;

    // some housekeeping stuff


/*-----------------------------------------------------------------------------------------------*/
/*                                          Memory Type                                          */
/*-----------------------------------------------------------------------------------------------*/

    usableFirstPageAddress = (void*)(multiboot_meminfo->mem_lower * 1024);
    usableLastPageAddress = (void*) (multiboot_meminfo->mem_upper * 1024 + (uint32_t)usableFirstPageAddress);
    usable_first_address = heapStart;
    usable_last_address = usableLastPageAddress;

    // malloc init, clear multiboot2 information and other data
    memset(heapStart, 0, usableLastPageAddress + 4095 - (uintptr_t) heapStart);

/*-----------------------------------------------------------------------------------------------*/
/*                                          Paging Init                                          */
/*-----------------------------------------------------------------------------------------------*/

    // Initialization of paging structures
    // Map all of the paging structures to memory
    // Only enable kernel space memory for now, enable MMIO and user space later in init process

    // Init pageDirectoryCR3
    pageDirectoryCR3.address = (uint32_t)&pageDirectory >> 12;
    pageDirectoryCR3.pageWriteThrough = 1;
    pageDirectoryCR3.pageCacheDisable = 1;

    // Init pageDirectory
    void* currentPageIndex;
    for (int i = 0; i < 1024; i++) {
        // Separate User space and kernel space tables
        if ((uint32_t)currentPageIndex < (uint32_t)ustart) {        // Kernel space
            currentPageIndex = (void*)(i * 1024 * 4096);
            pageDirectory[i].present = 1;
            pageDirectory[i].RW = 1;
            pageDirectory[i].US = 0;
            pageDirectory[i].pageWriteThrough = 1;
            pageDirectory[i].pageCacheDisable = 1;
            pageDirectory[i].size = 1;
            pageDirectory[i].address = (uint32_t)&pageTable[i] >> 12;

            // Init kernel space page tables
            for (int j = 0; j < 1024; j++) {
                currentPageIndex = (void*)(i * 1024 * 4096 + j * 4096);
                pageTable[i][j].present = 1;
                pageTable[i][j].RW = 1;
                pageTable[i][j].US = 0;
                pageTable[i][j].pageWriteThrough = 1;
                pageTable[i][j].pageCacheDisable = 1;
                pageTable[i][j].global = 1;                         // Will set to global cuz lazy to implement TLBs
                pageTable[i][j].address = ((uint32_t)currentPageIndex) >> 12;
            }
        }

        else if (currentPageIndex <= usableLastPageAddress) {       // User space
            currentPageIndex = (void*)(i * 1024 * 4096);
            pageDirectory[i].present = 0;
            pageDirectory[i].RW = 1;
            pageDirectory[i].US = 1;
            pageDirectory[i].pageWriteThrough = 1;
            pageDirectory[i].pageCacheDisable = 1;
            pageDirectory[i].size = 1;
            pageDirectory[i].address = (uint32_t)&pageTable[i] >> 12;

            // Init user space page tables
            for (int j = 0; j < 1024; j++) {
                currentPageIndex = (void*)(i * 1024 * 4096 + j * 4096);
                pageTable[i][j].present = 0;
                pageTable[i][j].RW = 1;
                pageTable[i][j].US = 1;
                pageTable[i][j].pageWriteThrough = 1;
                pageTable[i][j].pageCacheDisable = 1;
                pageTable[i][j].global = 1;                         // Will set to global cuz lazy to implement TLBs
                pageTable[i][j].address = ((uint32_t)currentPageIndex) >> 12;
            }
        }

        else if (currentPageIndex <= finalPageAddress) {            // MMIO address magic
            currentPageIndex = (void*)(i * 1024 * 4096);
            pageDirectory[i].present = 0;
            pageDirectory[i].RW = 1;
            pageDirectory[i].US = 0;
            pageDirectory[i].pageWriteThrough = 1;
            pageDirectory[i].pageCacheDisable = 1;
            pageDirectory[i].size = 1;
            pageDirectory[i].address = (uint32_t)&pageTable[i] >> 12;

            // Init MMIO space page tables
            for (int j = 0; j < 1024; j++) {
                currentPageIndex = (void*)(i * 1024 * 4096 + j * 4096);
                pageTable[i][j].present = 0;
                pageTable[i][j].RW = 1;
                pageTable[i][j].US = 0;
                pageTable[i][j].pageWriteThrough = 1;
                pageTable[i][j].pageCacheDisable = 1;
                pageTable[i][j].global = 1;                         // Will set to global cuz lazy to implement TLBs
                pageTable[i][j].address = ((uint32_t)currentPageIndex) >> 12;
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

bool memory_if_exists(void* addr) {
    for (int i = 0; i < 1024; i++) {
    }
    return false;
}