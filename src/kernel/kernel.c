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
#include "print.h"
#include "memory.h"
#include "string.h"
#include "panic.h"
#include "interrupt.h"



void kernel_init(multiboot_info_t* mbd, uint32_t magic, void* heapStart, uint16_t cs) {
    char tempString[64];
/*-----------------------------------------------------------------------------------------------*/
/*                                        Physical Memory                                        */
/*-----------------------------------------------------------------------------------------------*/
    // Serial
    serialInit(COM1);
    // Init memory
    print("Initializing memory... ");
    if (!memory_init(mbd, magic, heapStart)) {
        println("OK");
    }
    else {
        println ("Error Initializing Memory");
        panic();
    }

/*-----------------------------------------------------------------------------------------------*/
/*                                         Virtual Memory                                        */
/*-----------------------------------------------------------------------------------------------*/
    interrupt_init(cs);
    println("interrupts");
    asm ("int $32");
    
    println("Welcome to miniOS!");
    while (1) {
        println("stop");
    }
}


