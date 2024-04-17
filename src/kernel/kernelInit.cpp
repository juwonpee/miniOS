#include "include/stdint-gcc.hpp"
#include "include/multiboot2.hpp"
#include "driver/serial.hpp"
#include "include/string.hpp"
#include "kernel/error.hpp"
#include "kernel/interrupt.hpp"

struct multibootTagHeader_t {
    multiboot_uint32_t total_size;
    multiboot_uint32_t reserved;
};

struct multiboot_tag_basic_meminfo* multiboot_meminfo;
struct multiboot_tag_old_acpi* multiboot_acpi;

bool multibootCheck(uint32_t magicNumber, struct multibootTagHeader_t* multibootTagHeader) {
    /* Make sure the magic number matches for memory mapping*/
    if(magicNumber != MULTIBOOT2_BOOTLOADER_MAGIC) {
        serial::printf("Fatal Error: Invalid multiboot2 magic number");
        serial::printf("Check if booted by multiboot2 compliant bootloader");
        serial::printf("%x", magicNumber);
        return false;
    }

    if ((uintptr_t)multibootTagHeader & 7) {
        serial::printf("Unaligned MBI");       // Whatever mbi is
        return true;
    }

    for (
        struct multiboot_tag* mbi = (struct multiboot_tag*)((uintptr_t)(multibootTagHeader) + sizeof(struct multibootTagHeader_t)); 
        (uintptr_t)mbi < multibootTagHeader -> total_size + (uintptr_t)multibootTagHeader; 
        mbi = (struct multiboot_tag*)(((uintptr_t)mbi) + mbi -> size)
    ) {
        // 8 byte alignment
        if ((uintptr_t)mbi % 8 != 0) {
            mbi = (struct multiboot_tag*)(((uintptr_t)mbi & 0xFFFFFFF8) + 0x8);
        }
        switch (mbi -> type) {
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                serial::printf("Bootloader: ");
                serial::printf(((struct multiboot_tag_string*)mbi) -> string);
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                multiboot_meminfo = (struct multiboot_tag_basic_meminfo*)mbi;
                break;
            case MULTIBOOT_TAG_TYPE_ACPI_OLD:
                multiboot_acpi = (struct multiboot_tag_old_acpi*)mbi;
                break;
        }    
    }
    return true;
}

extern "C" void kernelInit(uint32_t magicNumber, struct multibootTagHeader_t multibootTag, void* heapStartmultibootTagHeaderess) {
    serial::serialInit();

    serial::printf("Checking boot information\n");
    // if (multibootCheck(magicNumber, &multibootTag)) {
    //     serial::printf("Good");
    // }
    // else {
    //     error::panic("Multiboot info not correct\n");
    // }

    serial::printf("Initializing interrupts\n");
    {
        uint16_t codeSegment;
        asm volatile (
            "mov %%cs, %0" 
            : "=r" (codeSegment)
        );
        interrupt::init(codeSegment);
    }


    serial::printf("Welcome to miniOS");
    return; 
}