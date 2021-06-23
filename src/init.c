#include <stdbool.h>


#include "include/types.h"
#include "print/print.h"
#include "include/multiboot.h"

void panic() {
    println("Halting");
    __asm__(
        "hlt"
    );
}






//------------------------PHYSICAL ADDRESS SPACE------------------------
//
//
//------------------------PHYSICAL ADDRESS SPACE------------------------

void init(multiboot_info_t *mbd, uint32_t magic) {
    set_graphics_mode(GRAPHICS_VGA_TEXT_80_25);
    println("Welcome to miniOS");

    println("Checking memory");
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        println("Not booted using multiboot");
        panic();
    }
    if (!(mbd -> flags >> 6 & 0x1)) {
        println("Invalid memory map provided by grub");
        panic();
    }

    for(uint32_t i = 0; i < mbd->mmap_length; 
        i += sizeof(multiboot_memory_map_t)) 
    {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
 
        println("Start Addr: %x | Length: %x | Size: %x | Type: %d", mmmt->addr, mmmt->len, mmmt->size, mmmt->type);
        print("start address: ");
        print
 
        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            /* 
             * Do something with this memory block!
             * BE WARNED that some of memory shown as availiable is actually 
             * actively being used by the kernel! You'll need to take that
             * into account before writing to memory!
             */
        }
    }
}
