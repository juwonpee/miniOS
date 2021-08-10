#include "memory.h"



void memory_init(multiboot_info_t* mbd, uint32_t magic) {
    // temp string of size 64
    char tempString[64];

    println("Initializing memory");

    /* Make sure the magic number matches for memory mapping*/
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        println("invalid magic number!");
    }
 
    /* Check bit 6 to see if we have a valid memory map */
    if(!(mbd->flags >> 6 & 0x1)) {
        println("invalid memory map given by GRUB bootloader");
    }
 
    /* Loop through the memory map and display the values */
    int i;
    for(i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) 
    {
        multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
        
        print("Start Addr : ");
        println(itoa(mmmt->addr, tempString, 16));
        print("Length: ");
        println(itoa(mmmt->len, tempString, 16));
        print("Size: ");
        println(itoa(mmmt->size, tempString, 16));
        print("type: ");
        println(itoa(mmmt->type, tempString, 16));
        // printf("Start Addr: %x | Length: %x | Size: %x | Type: %d\n",
        //     mmmt->addr, mmmt->len, mmmt->size, mmmt->type);
 
        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            /* 
             * Do something with this memory block!
             * BE WARNED that some of memory shown as availiable is actually 
             * actively being used by the kernel! You'll need to take that
             * into account before writing to memory!
             */
            print("Memory block: ");
            println(itoa(i, tempString, 10));
        }

        println("");
    }
}

int bbmalloc(uint32_t size) {
    
}