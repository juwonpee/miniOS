#include "types.h"
#include "print.h"
#include "memory.h"
#include "string.h"

//------------------------PHYSICAL ADDRESS SPACE------------------------


void kernel_init(multiboot_info_t* mbd, uint32_t magic) {

    char testString[64];
    // Serial
    int test = serialInit(COM1);
    memory_init(mbd, magic);
    println("Welcome to miniOS!");

}


