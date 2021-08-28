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

#include <stdalign.h>

#include "types.h"

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

typedef struct IDT_t {
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
} __attribute__ ((packed)) IDT_t;

typedef struct IDTR_t {
	uint16_t limit;
	uint32_t base;
} __attribute__ ((packed)) IDTR_t;

bool idt_init();
bool pic_init();
bool interrupt_init();


extern void isr000();
extern void isr001();
extern void isr002();
extern void isr003();
extern void isr004();
extern void isr005();
extern void isr006();
extern void isr007();
extern void isr008();
extern void isr009();
extern void isr010();
extern void isr011();
extern void isr012();
extern void isr013();
extern void isr014();
extern void isr015();
extern void isr016();
extern void isr017();
extern void isr018();
extern void isr019();
extern void isr020();
extern void isr021();
extern void isr022();
extern void isr023();
extern void isr024();
extern void isr025();
extern void isr026();
extern void isr027();
extern void isr028();
extern void isr029();
extern void isr030();
extern void isr031();
extern void isr032();
extern void isr033();
extern void isr034();
extern void isr035();
extern void isr036();
extern void isr037();
extern void isr038();
extern void isr039();
extern void isr040();
extern void isr041();
extern void isr042();
extern void isr043();
extern void isr044();
extern void isr045();
extern void isr046();
extern void isr047();
extern void isr048();
extern void isr049();
extern void isr050();
extern void isr051();
extern void isr052();
extern void isr053();
extern void isr054();
extern void isr055();
extern void isr056();
extern void isr057();
extern void isr058();
extern void isr059();
extern void isr060();
extern void isr061();
extern void isr062();
extern void isr063();
extern void isr064();
extern void isr065();
extern void isr066();
extern void isr067();
extern void isr068();
extern void isr069();
extern void isr070();
extern void isr071();
extern void isr072();
extern void isr073();
extern void isr074();
extern void isr075();
extern void isr076();
extern void isr077();
extern void isr078();
extern void isr079();
extern void isr080();
extern void isr081();
extern void isr082();
extern void isr083();
extern void isr084();