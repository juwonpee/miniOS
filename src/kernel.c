#include "types.h"
#include "print.h"
#include "memory.h"
#include "string.h"
#include "panic.h"


void initCPU() {
    // Enable cache
    // Enable FPU unit
}


void kernel_init(multiboot_info_t* mbd, uint32_t magic) {
    // Serial
    serialInit(COM1);
    // Init memory
    println("Initializing memory... ");
    if (!memory_init(mbd, magic)) {
        println("OK");
    }
    else {
        println ("Error");
        panic();
    }
    
    println("Welcome to miniOS!");

}


