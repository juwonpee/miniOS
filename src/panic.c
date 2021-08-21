#include "panic.h"

void panic() {
    println("Halting");
    asm volatile ("hlt");
}