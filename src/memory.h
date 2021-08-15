#pragma once

#include <stdalign.h>

#include "types.h"
#include "multiboot.h"
#include "print.h"
#include "string.h"

#define KHEAP_START 0x1000000   // leave 16MB for kernel code (direct paging)
                                // 64MB for kernel heap data
#define UHEAP_START 0x5000000   // from 64MB for userland code heap paging
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
} pageDirectoryCR3_t;

typedef struct pageDirectory_t {
    union {
        uint32_t data;
        struct {
            uint32_t present:1;
            uint32_t RW:1;                                          // Read/Write
            uint32_t US:1;                                          // User/Supervisor
            uint32_t pageWriteThrough:1;
            uint32_t pageCacheDisable:1;
            uint32_t accessed:1;
            uint32_t ignore1:1;
            uint32_t size:1;                                        // 0: 4MB pages, 1: 4KiB pages
            uint32_t ignore2:4;
            uint32_t address:20;
        };
    };
} pageDirectory_t;

typedef struct pageTable_t {
    union {
        uint32_t data;
        union{
            uint32_t present:1;
            uint32_t RW:1;                                         // Read/Write
            uint32_t US:1;                                         // User/Supervisor
            uint32_t writeThrough:1;
            uint32_t cacheDisable:1;
            uint32_t accessed:1;
            uint32_t dirty:1;                                       // 0: 4MB pages, 1: 4KiB pages
            uint32_t PAT:1;                                         // PAT something i dont know, must stay 0
            uint32_t ignore:1;
            uint32_t available:3;
            uint32_t address:20;
        };
    };
} pageTable_t;





void memory_init(multiboot_info_t* mbd, uint32_t magic);

void* bbmalloc(uint32_t size);

void bbfree(void* address);

void* kmalloc(uint32_t size);