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

#include "types.h"
#include "io.h"
#include "ata.h"
#include "interrupt.h"

#define PIT_DIVISOR                 8192
#define PIT_MAX_DIVISOR             65535
#define PIT_BASE_FREQUENCY          1193182

#define PIT_COMMAND                 0x40


bool pit_init();

bool pit_increment_time();
uint64_t pit_get_time_since_boot();