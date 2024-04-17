#include "kernel/error.hpp"
#include "driver/serial.hpp"

void error::panic(const char* reason) {
    serial::printf(reason);

    asm volatile (
        "cli"
    );

    asm volatile (
        "hlt"
    );
}

void error::warning(const char* reason) {
    serial::printf("Warning: %s", reason);
}