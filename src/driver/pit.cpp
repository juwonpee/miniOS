#include "driver/pit.hpp"
#include "driver/serial.hpp"
#include "driver/io.hpp"
#include "kernel/error.hpp"

uint64_t pitTimeSinceBoot = 0;
uint32_t pitFrequency;
uint64_t pitTicks = 0;

void pit::pitInit() {
    uint16_t divisor = PIT_DIVISOR;
    if (divisor > PIT_MAX_DIVISOR) {
        divisor = PIT_MAX_DIVISOR;
        error::warning("PIT_DIVISOR larger than PIT_MAX_DIVISOR, Reccomend changing value\n");
    }
    io::outb(0x40, (uint8_t)(divisor & 0xFF));
    io::outb(0x40, (uint8_t)(divisor >> 8));

    pitFrequency = PIT_BASE_FREQUENCY / divisor;

    interrupt::registerISR(32, isr, IDT_FLAGS_INTERRUPT);
}

void pit::incrementTicks() {
    pitTicks++;
}

uint64_t pit::getTimeSinceBootMillis() {
    return pitTicks * pitFrequency * 1000 / PIT_BASE_FREQUENCY;
}

void pit::isr(IDTInterruptFrame_t* interruptFrame) {
    interrupt::disable();

    incrementTicks();
    serial::printf("%x", getTimeSinceBootMillis());

    interrupt::picReturn(32);

    interrupt::enable();
}

void pit::interruptEnd(uint8_t irq) {
}