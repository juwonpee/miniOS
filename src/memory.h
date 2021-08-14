#pragma once

#include "types.h"
#include "multiboot.h"
#include "print.h"
#include "string.h"

#define KHEAP_START 0x1000000   // leave 16MB for kernel code (direct paging)
                                // 64MB for kernel heap data
#define UHEAP_START 0x5000000   // from 64MB for userland code heap paging
#define nullptr (void*)0x0


static uint32_t* kstart = (void*) KHEAP_START;
static uint32_t* ustart = (void*) UHEAP_START;



void memory_init(multiboot_info_t* mbd, uint32_t magic);

void* bbmalloc(uint32_t size);

void bbfree(void* address);

void* kmalloc(uint32_t size);