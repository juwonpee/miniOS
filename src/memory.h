#include "types.h"


#define HEAP_START 0x400000

void* heap_pointer = HEAP_START

void* kmalloc(uint32_t size);

void* kmalloc_a(uint32_t size, uint32_t align);

void* kmalloc_a