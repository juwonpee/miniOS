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

typedef struct interruptFrame_t interruptFrame_t;

void interrupt_idt_init(uint16_t cs);
void interrupt_pic_init();
void interrupt_enable();
void interrupt_disable();
bool interrupt_init(uint16_t cs);


// __attribute__ ((interrupt)) void isr000(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr001(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr002(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr003(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr004(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr005(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr006(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr007(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr008(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr009(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr010(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr011(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr012(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr013(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr014(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr015(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr016(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr017(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr018(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr019(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr020(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr021(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr022(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr023(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr024(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr025(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr026(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr027(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr028(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr029(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr030(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr031(interruptFrame_t* interruptFrame);

__attribute__ ((interrupt)) void interrupt_irq032(interruptFrame_t* interruptFrame);

// __attribute__ ((interrupt)) void isr033(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr034(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr035(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr036(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr037(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr038(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr039(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr040(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr041(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr042(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr043(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr044(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr045(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr046(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr047(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr048(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr049(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr050(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr051(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr052(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr053(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr054(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr055(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr056(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr057(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr058(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr059(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr060(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr061(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr062(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr063(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr064(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr065(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr066(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr067(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr068(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr069(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr070(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr071(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr072(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr073(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr074(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr075(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr076(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr077(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr078(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr079(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr080(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr081(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr082(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr083(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void isr084(interruptFrame_t* interruptFrame);