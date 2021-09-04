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
#include "print.h"
#include "panic.h"

#define PIC1					0x20		/* IO base address for master PIC */
#define PIC2					0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND			PIC1
#define PIC1_DATA				(PIC1+1)
#define PIC2_COMMAND			PIC2
#define PIC2_DATA				(PIC2+1)

typedef struct IDT_t {
	union {
		uint64_t data;
		struct {
			uint64_t offset1:16;
			uint64_t selector:16;
			uint64_t ignore:8;
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

void interrupt_idt_init();
void interrupt_pic_init();
void interrupt_enable();
void interrupt_disable();
bool interrupt_init();


// void isr000();
// void isr001();
// void isr002();
// void isr003();
// void isr004();
// void isr005();
// void isr006();
// void isr007();
// void isr008();
// void isr009();
// void isr010();
// void isr011();
// void isr012();
// void isr013();
// void isr014();
// void isr015();
// void isr016();
// void isr017();
// void isr018();
// void isr019();
// void isr020();
// void isr021();
// void isr022();
// void isr023();
// void isr024();
// void isr025();
// void isr026();
// void isr027();
// void isr028();
// void isr029();
// void isr030();
// void isr031();

void interrupt_isr032(void);

// void isr033();
// void isr034();
// void isr035();
// void isr036();
// void isr037();
// void isr038();
// void isr039();
// void isr040();
// void isr041();
// void isr042();
// void isr043();
// void isr044();
// void isr045();
// void isr046();
// void isr047();
// void isr048();
// void isr049();
// void isr050();
// void isr051();
// void isr052();
// void isr053();
// void isr054();
// void isr055();
// void isr056();
// void isr057();
// void isr058();
// void isr059();
// void isr060();
// void isr061();
// void isr062();
// void isr063();
// void isr064();
// void isr065();
// void isr066();
// void isr067();
// void isr068();
// void isr069();
// void isr070();
// void isr071();
// void isr072();
// void isr073();
// void isr074();
// void isr075();
// void isr076();
// void isr077();
// void isr078();
// void isr079();
// void isr080();
// void isr081();
// void isr082();
// void isr083();
// void isr084();