#pragma once

#include "types.h"
#include "multiboot.h"
#include "print.h"
#include "string.h"

#define HEAP_START 0x400000


void memory_init(multiboot_info_t* mbd, uint32_t magic);

void* bbmalloc(uint32_t size);