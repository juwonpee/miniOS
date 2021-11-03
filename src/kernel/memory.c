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


void* malloc(uintptr_t size) {
    // TODO: If first node gets freed, everything breaks cuz the references to the second node are broken

    // All memory allocations are word size aligned cuz speed
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

    memory_malloc_node_t* node = heap_start;
    memory_malloc_node_t* lastNode = node;
    while (1) {
        if (node->size == 0) {
            // Insert new node to end of list
            node->size = size + sizeof(memory_malloc_node_t);
            if (lastNode != node) {
                node->prevNode = lastNode;
                lastNode->nextNode = node;
            }
            return &node->data;
        }
        else {
            if (node->nextNode != 0) {
                if ((uintptr_t)node->nextNode - (uintptr_t)node - node->size >= size + sizeof(memory_malloc_node_t)) {
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

                    return &node->data;
                }
            }
            lastNode = node;
            node = (memory_malloc_node_t*)(node->size + (uintptr_t)node);
        }
    }
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
    malloc(4);																					// TEMPFIX: Because of the first node problem of malloc, need an unreferenced first node to keep the references for the second node

/*-----------------------------------------------------------------------------------------------*/
/*                                          Paging Init                                          */
/*-----------------------------------------------------------------------------------------------*/
    
    char hello[] = "Hello World!";

    char* test1 = malloc(16);
    memcpy(test1, hello, 13);

    char* test2 = malloc(16);
    memcpy(test2, hello, 13);

    char* test3 = malloc(16);
    memcpy(test3, hello, 13);

    free(test2);

    test2 = malloc(8);
    free(test2);

    test2 = malloc(32);

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
                }
            }
            else {
                pageDirectory[i].present = 0;
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