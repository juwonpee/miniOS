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
struct multiboot_tag_old_acpi* multiboot_acpi;
acpi_master_table_t acpi_master_table;

bool bootInfo(uint32_t magic, struct multiboot_tag_header* addr) {
    char tempString[64];

    /* Make sure the magic number matches for memory mapping*/
    if(magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        println("Fatal Error: Invalid multiboot2 magic number");
        print("Check if booted by multiboot2 compliant bootloader");
        println(itoa(magic, tempString, 16));
        return ATA_TRUSTED_RECIEVE;
    }

    if ((uintptr_t)addr & 7) {
        println("Unaligned MBI");       // Whatever mbi is
        return true;;
    }

    for (
        struct multiboot_tag* mbi = (struct multiboot_tag*)((uintptr_t)(addr) + sizeof(struct multiboot_tag_header)); 
        (uintptr_t)mbi < addr -> total_size + (uintptr_t)addr; 
        mbi = (struct multiboot_tag*)(((uintptr_t)mbi) + mbi -> size)
    ) {
        // 8 byte alignment
        if ((uintptr_t)mbi % 8 != 0) {
            mbi = (struct multiboot_tag*)(((uintptr_t)mbi & 0xFFFFFFF8) + 0x8);
        }
        switch (mbi -> type) {
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                print("Bootloader: ");
                println(((struct multiboot_tag_string*)mbi) -> string);
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                multiboot_meminfo = (struct multiboot_tag_basic_meminfo*)mbi;
                break;
            case MULTIBOOT_TAG_TYPE_ACPI_OLD:
                multiboot_acpi = (struct multiboot_tag_old_acpi*)mbi;
                break;
        }    
    }
    return false;
}

void kernel_init(uint32_t magic, struct multiboot_tag_header* addr, void* heapStart) {
/*-----------------------------------------------------------------------------------------------*/
/*                                        Physical Memory                                        */
/*-----------------------------------------------------------------------------------------------*/
    // Serial
    serialInit();

    // Check GRUB/EFI system tables
    print("Checking Boot Information... ");
    if (!bootInfo(magic, addr)) {
        println("OK");
    }
    else {
        println("Error while checking Boot Information");
        panic();
    }

    print("Initializing ACPI tables... ");
    acpi_master_table = acpi_init(multiboot_acpi);
    if (!acpi_master_table.OK) {
        println("OK");
    }
    else {
        println("Error Initializing ACPI");
        panic();
    }

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

    // print("Initializing PCI bus... ");
    // if (!pci_init(acpi_master_table)) {
    //     println("OK");
    // }
    // else {
    //     println("Error Initializing PCI bus");
    //     panic();
    // }

    print("Initializing Drive.. ");
    if (!ata_init()) {
        println("OK");
    }
    else {
        println ("Error Initializing Drive");
        panic();
    }
    ata_sector_data_t data = ata_secondary_read(1);
    data.data[512] = '\0';
    println((char*)data.data);
    
    println("Welcome to miniOS!");
    while(1) {
        
    }
}


