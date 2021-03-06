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

#include "sched.h"

scheduler_pid_t context = 0;

scheduler_status_t status = normal;

uintptr_t uninterruptable_counter = 0;



void scheduler_kernel_uninterruptible() {
   if (uninterruptable_counter == 0) {
      status = uninterruptible;
	   interrupt_disable();
   }
   uninterruptable_counter++;
}

void scheduler_kernel_interruptible() {
   uninterruptable_counter--;
   if (uninterruptable_counter == 0) {
      interrupt_enable();
      status = interruptible;
   }
}

scheduler_pid_t scheduler_get_context() {
	return context;
}