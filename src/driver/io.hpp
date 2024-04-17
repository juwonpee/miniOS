#include "include/stdint-gcc.hpp"

#pragma once
class io {
    public:
    static void outb(uint32_t port, uint8_t data);

    static uint8_t inb(uint32_t port);
};