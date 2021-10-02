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
#include "kernel.h"
#include "io.h"
#include "print.h"
#include "memory.h"
#include "string.h"
#include "panic.h"
#include "interrupt.h"
#include "pci.h"
#include "multiboot2.h"
#include "acpi.h"
//#include "ata.h"

struct multiboot_tag_basic_meminfo* multiboot_meminfo;
struct multiboot_tag_new_acpi* multiboot_acpi;

void bootInfo(uint32_t magic, uint32_t addr) {
    char tempString[64];

    /* Make sure the magic number matches for memory mapping*/
    if(magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        println("Fatal Error: Invalid multiboot2 magic number");
        print("Check if booted by multiboot2 compliant bootloader");
        println(itoa(magic, tempString, 16));
        panic();
    }

    if (addr & 7) {
        println("Unaligned MBI");       // Whatever mbi is
        panic();
    }
    for (
        struct multiboot_tag* mbi = (struct multiboot_tag*)(addr + sizeof(struct multiboot_header)); 
        (uintptr_t)mbi < addr + ((struct multiboot_header*)addr) -> header_length; 
        mbi = (struct multiboot_tag*)((uintptr_t)mbi + mbi -> size)
    ) {
        switch (mbi -> type) {
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                println("MULTIBOOT TAG TYPE BASIC MEMINFO");
                multiboot_meminfo = (struct multiboot_tag_basic_meminfo*)mbi;
                break;
            case MULTIBOOT_TAG_TYPE_ACPI_NEW:
                println("MULTIBOOT TAG TYPE ACPI NEW");
                multiboot_acpi = (struct multiboot_tag_new_acpi*)mbi;
                break;
        }    
    }
}

void kernel_init(uint32_t addr, uint32_t magic, void* heapStart) {
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
    uint16_t cs;
    asm volatile (
        "mov %%cs, %0" 
        : "=r" (cs)
        : 
    );
    if (!interrupt_init(cs)) {
        println("OK");
    }
    else {
        println ("Error Initializing Interrupts");
        panic();
    }

    print("Initializing ACPI tables... ");
    if (!acpi_init(multiboot_acpi)) {
        println("OK");
    }
    else {
        println("Error Initializing ACPI");
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


