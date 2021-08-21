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


