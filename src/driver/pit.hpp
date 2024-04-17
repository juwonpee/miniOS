#pragma once

#include "stdint-gcc.hpp"
#include "kernel/interrupt.hpp"

#define PIT_DIVISOR                 65535
#define PIT_MAX_DIVISOR             65535
#define PIT_BASE_FREQUENCY          1193182

#define PIT_COMMAND                 0x40






class pit {
    public:
    static void pitInit();
    static void incrementTicks();
    
    // Not properly inplemented as to no accurate timers
    static uint64_t getTimeSinceBootMillis();

    __attribute__((interrupt)) static void isr(IDTInterruptFrame_t* interruptFrame);
    static void interruptEnd(uint8_t irq);
};