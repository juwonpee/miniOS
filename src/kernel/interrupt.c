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

void interrupt_idt_init(uint16_t cs) {
	IDTR.limit = 256 * sizeof(IDT_t) - 1;
	IDTR.base = (uint32_t)&IDT;

	// ISR address 0x00~0x1F are CPU reserved in protected mode
	IDT[8].offset1 = (uint32_t)&interrupt_isr008 & 0xFFFF;
	IDT[8].selector = cs;
	IDT[8].ignore = 0;
	IDT[8].flags = IDT_FLAGS_TRAP;
	IDT[8].offset2 = (uint32_t)&interrupt_isr008 >> 16;

	IDT[32].offset1 = (uint32_t)&interrupt_irq032 & 0xFFFF;
	IDT[32].selector = cs;
	IDT[32].ignore = 0;
	IDT[32].flags = IDT_FLAGS_INTERRUPT;
	IDT[32].offset2 = (uint32_t)&interrupt_irq032 >> 16;
	
	// Load IDTR into CPU
	asm volatile (
		"lidt %0"
		:
		: "m"(IDTR)
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

bool interrupt_init(uint16_t cs) {
	interrupt_disable();
	interrupt_idt_init(cs);
	interrupt_pic_init();
	interrupt_enable();
	return false;
}

__attribute__ ((interrupt)) void interrupt_isr008(interruptFrame_t* interruptFrame) {
	println("Interrupt: 008");
}

__attribute__ ((interrupt)) void interrupt_irq032(interruptFrame_t* interruptFrame) {
	// test interrupt
	println("Interrupt: 032");
}