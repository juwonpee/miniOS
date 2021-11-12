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
#include "interrupt.h"

#define SCHED_PID_KERNEL      0
#define SCHED_PID_NOTHING     0xFFFF


void scheduler_init();

void scheduler_kernel_uninterruptible();

void scheduler_kernel_interruptible();

scheduler_context_t scheduler_get_context();

void scheduler_interrupt_handler();