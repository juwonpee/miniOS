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



#include "types.h"
#include "io.h"
#include "print.h"
#include "memory.h"
#include "string.h"
#include "panic.h"
#include "interrupt.h"
#include "pci.h"
#include "multiboot2.h"
//#include "ata.h"

struct multiboot_tag_basic_meminfo* multiboot_meminfo;
struct multiboot_tag_new_acpi* multiboot_acpi;

void bootInfo(uint32_t magic, uint32_t addr) {
    char* tempString[64];

    /* Make sure the magic number matches for memory mapping*/
    if(magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        println("Fatal Error: Invalid magic number");
        print("Check multiboot magic number: ");
        println(itoa(magic, tempString, 16));
        panic();
    }

    if (addr & 7) {
        println("Unaligned MBI");       // Whatever mbi is
        panic();
    }
    for (struct multiboot_tag* mbi = (struct multiboot_tag*)(addr + 8); mbi -> type != MULTIBOOT_TAG_TYPE_END; mbi = (struct multiboot_tag *) ((multiboot_uint8_t *) mbi + ((mbi->size + 7) & ~7))) {
        switch (mbi -> type) {
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                multiboot_meminfo = (struct multiboot_tag_basic_meminfo*)mbi;
                break;
            case MULTIBOOT_TAG_TYPE_ACPI_NEW:
                multiboot_acpi = (struct multiboot_tag_new_acpi*)mbi;
                break;
        }
    }
}

void kernel_init(uint32_t magic, uint32_t addr, void* heapStart, uint16_t cs) {
/*-----------------------------------------------------------------------------------------------*/
/*                                        Physical Memory                                        */
/*-----------------------------------------------------------------------------------------------*/
    // Serial
    serialInit();

    // Check GRUB/EFI system tables
    bootInfo(magic, addr);

    // Init memory
    print("Initializing memory... ");
    if (!memory_init(multiboot_meminfo, heapStart)) {
        println("OK");
    }
    else {
        println ("Error Initializing Memory");
        panic();
    }

/*-----------------------------------------------------------------------------------------------*/
/*                                         Virtual Memory                                        */
/*-----------------------------------------------------------------------------------------------*/
    print("Initializing Interrupts... ");
    if (!interrupt_init(cs)) {
        println("OK");
    }
    else {
        println ("Error Initializing Interrupts");
        panic();
    }

    print("Initializing PCI bus... ");
    if (!pci_init(multiboot_acpi)) {
        println("OK");
    }
    else {
        println("Error Initializing PCI bus");
        panic();
    }

    // print("Initializing Drive.. ");
    // if (!ata_init()) {
    //     println("OK");
    // }
    // else {
    //     println ("Error Initializing Drive");
    //     panic();
    // }
    
    println("Welcome to miniOS!");
    while(1) {
        
    }
}


