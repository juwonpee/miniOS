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

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

typedef struct IDTDescriptor_t {
	union {
		uint64_t data;
		struct {
			uint64_t offset1:16;
			uint64_t selector:16;
			uint64_t ignore1:8;
			uint64_t gate:4;
			uint64_t segment:1;
			uint64_t privilege:2;
			uint64_t present:1;
			uint64_t offset2:16;
		};
	};
} __attribute__ ((packed)) IDTDescriptor_t;

bool idt_init();
bool pic_init();
bool interrupt_init();