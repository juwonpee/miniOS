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



#include "interrupt.h"

alignas(16) IDT_t IDT[256];                                       // Aligned for performance, not required
IDTR_t IDTR;

void interrupt_idt_init() {
	IDTR.base = (uint32_t)&IDT;
	IDTR.limit = 256 * sizeof(IDT_t) - 1;

	// ISR address 0x00~0x1F are CPU reserved in protected mode
	IDT[32].offset1 = (uint32_t)&interrupt_isr032 & 0xFFFF;
	IDT[32].selector = 0;
	IDT[32].ignore = 0;
	IDT[32].gate = 0xE;
	IDT[32].segment = 0;
	IDT[32].privilege = 0;
	IDT[32].present = 1;
	IDT[32].offset2 = (uint32_t)&interrupt_isr032 >> 16;
	
	asm volatile (
		"lidt %0"
		:
		: "memory"(IDTR)
	);
}

void interrupt_pic_init() {
}

void interrupt_enable() {
	asm ("sti");
}

void interrupt_disable() {
	asm ("cli");
}

bool interrupt_init() {
	interrupt_disable();
	interrupt_idt_init();
	interrupt_pic_init();
	interrupt_enable();
	return false;
}

void interrupt_isr032() {
	// test interrupt
	asm ("pushal");
	println("Interrupt!");
	asm ("popal; leave; iret");
}