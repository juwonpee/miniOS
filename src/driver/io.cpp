#include "driver/io.hpp"

void io::outb(uint32_t port, uint8_t data) {

        asm volatile (
            "outb %0, %1"
        : 
        : "a" (data), "Nd" (port)
        );
}

uint8_t io::inb(uint32_t port) {
    uint8_t returnVal;
    asm volatile (
        "inb %1, %0" 
        : "=a" (returnVal)
        : "Nd" (port)
    );
    return returnVal;
}