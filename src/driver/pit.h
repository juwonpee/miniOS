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


#pragma once


#include "io.h"
#include "ata.h"
#include "interrupt.h"

#define PIT_DIVISOR                 65535
#define PIT_MAX_DIVISOR             65535
#define PIT_BASE_FREQUENCY          1193182

#define PIT_COMMAND                 0x40


bool pit_init();

__attribute__ ((fastcall)) bool pit_increment_time();

// Return seconds
uint64_t pit_get_time_since_boot_seconds();

// Not properly inplemented as to no accurate timers
uint64_t pit_get_time_since_boot_millis();