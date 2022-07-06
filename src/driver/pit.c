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

#include "pit.h"

uint64_t pit_time_since_boot_seconds = 0;
uint64_t pit_time_since_boot = 0;
uint32_t pit_frequency;
uint64_t pit_ticks = 0;

bool pit_init() {
    uint16_t divisor = PIT_DIVISOR;
    if (divisor > PIT_MAX_DIVISOR) {
        divisor = PIT_MAX_DIVISOR;
        println("Warning: PIT_DIVISOR larger than PIT_MAX_DIVISOR, Reccomend changing value");
    }
    outb(0x40, (uint8_t)(divisor & 0xFF));
    io_wait();
    outb(0x40, (uint8_t)(divisor >> 8));

    pit_frequency = PIT_BASE_FREQUENCY / divisor;
    return false;
}

__attribute__ ((fastcall)) bool pit_increment_time() {
    pit_ticks++;
    pit_time_since_boot++;
    if (pit_ticks == pit_frequency) {
        pit_time_since_boot_seconds++;
        pit_ticks = 0;
        return true;
    }
    return false;
}

uint64_t pit_get_time_since_boot_seconds() {
    return pit_time_since_boot_seconds;
}

uint64_t pit_get_time_since_boot_millis() {
    return pit_ticks * pit_frequency * 1000 / PIT_BASE_FREQUENCY;
}