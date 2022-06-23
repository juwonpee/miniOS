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
#include "memory.h"

#define SCHED_PID_KERNEL      0
// Kernel threads are defined between 0x0001 ~ 0x0FFF
#define SCHED_PID_NOTHING     0xFFFF


// Forward declarations
typedef struct memoryPageDirectoryCR3_t memoryPageDirectoryCR3_t;
typedef struct memoryPageDirectory_t memoryPageDirectory_t;
typedef struct memoryPageTable_t memoryPageTable_t;


typedef enum scheduler_status_t {
	interruptible,
	uninterruptible,
} scheduler_status_t;

typedef enum scheduler_thread_status_t {
	creating,		// While initializing thread
	ready,			// While in ready queue
	running,		// While currently running
	waiting,		// While waiting for signal from other thread
	delayed,		// While in sleep() system call
	blocked,		// While waiting for I/O
} scheduler_thread_status_t;

typedef struct scheduler_kernel_thread_t {
    // General purpose registers
	uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;

    // Scheduler specific
    scheduler_pid_t pid;
    uint32_t priority;
    uint32_t time;
    scheduler_thread_status_t status;
    struct scheduler_kernel_thread_t* prevTask;
    struct scheduler_kernel_thread_t* nextTask;
    char name[256];
} __attribute__ ((packed)) scheduler_kernel_thread_t;

typedef struct scheduler_user_thread_t {
    // General purpose registers
	uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;

    // Scheduler specific
    scheduler_pid_t pid;
    uint32_t priority;
    uint32_t time;
    scheduler_thread_status_t status;
    struct scheduler_user_thread_t* prevTask;
    struct scheduler_user_thread_t* nextTask;
    char name[256];

    // Memory specific
    memoryPageDirectoryCR3_t* pageDirectoryCR3;
    memoryPageDirectory_t* pageDirectory;
    memoryPageTable_t* pageTable;
} __attribute__ ((packed)) scheduler_user_thread_t;



void scheduler_init();

void scheduler_kernel_uninterruptible();

void scheduler_kernel_interruptible();

scheduler_pid_t scheduler_get_context();

void scheduler_switch_to(scheduler_user_thread_t);

void scheduler_interrupt_handler();

void scheduler_create_thread();

void schedule();
