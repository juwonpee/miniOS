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


uint32_t* kstart;
static uint32_t* ustart = (void*) UHEAP_START;

volatile pageDirectoryCR3_t pageDirectoryCR3;
alignas(4096) volatile pageDirectory_t pageDirectory[1024];
alignas(4096) volatile pageTable_t pageTable[1024][1024];

void* malloc(uint32_t size) {
    uint32_t* index = kstart;
    while ((uint32_t) index < (uint32_t) ustart) {
        if ((*index) == 0) {
            uint32_t* blockIndex = index + 1;
            uint32_t blockSize = 0;
            while ((uint32_t) blockIndex < (uint32_t)ustart) {
                if ((*blockIndex) == 0) {
                    blockSize += 4;
                    if (size <= blockSize) {
                        (*index) = size;
                        return (void*)(index + 1);
                    }
                    else {
                        blockIndex += 1;
                    }
                }
                else {
                    index += blockSize + 1;
                    break;
                }
            }
            
        }
        else {
            index += 1;
        }
    }

    // If out of bounds;
    println("Out of kernel memory, aborting malloc");
    return nullptr;
}

void free(void* address) {
    uint32_t* tempIndex = (uint32_t*)(address - 4);
    uint32_t tempSize = (*tempIndex);
    
    // clear out size data
    (*tempIndex) = 0;
    // clear out rest of block
    for (uint32_t size = 0; size < tempSize; size+=4) {
        tempIndex = address + size;
        (*tempIndex) = 0;
    }
}

bool memory_init(struct multiboot_tag_basic_meminfo* multiboot_meminfo, void* heapStart) {
    // TODO: BUG HACK
    // linker rounds down _end to the nearest 4096 byte, so there is still data at heapStart
    // skipping to another page to get clear memory
    heapStart += 4096;


    // temp string of size 64
    char tempString[64];
    // Variables to initialize memory;
    void* usableFirstPageAddress;
    void* usableLastPageAddress;
    void* finalPageAddress = (void*)0xFFFFF000;

    // some housekeeping stuff
    kstart = heapStart;

/*-----------------------------------------------------------------------------------------------*/
/*                                          Memory Type                                          */
/*-----------------------------------------------------------------------------------------------*/

    usableFirstPageAddress = (void*)(multiboot_meminfo->mem_lower * 1024);
    usableLastPageAddress = (void*) (multiboot_meminfo->mem_upper * 1024 + (uint32_t)usableFirstPageAddress);
    

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