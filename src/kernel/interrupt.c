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

alignas(16) IDT_t interrupt_IDT[256];                                       // Aligned for performance, not required
IDTR_t interrupt_IDTR;

uint64_t interrupt_Counter[256];

void interrupt_idt_init(uint16_t cs) {
	interrupt_IDTR.limit = 256 * sizeof(IDT_t) - 1;
	interrupt_IDTR.base = (uint32_t)&interrupt_IDT;

	// ISR address 0x00~0x1F are CPU reserved in protected mode
	interrupt_IDT[8].offset1 = (uint32_t)&interrupt_isr008 & 0xFFFF;
	interrupt_IDT[8].selector = cs;
	interrupt_IDT[8].ignore = 0;
	interrupt_IDT[8].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[8].offset2 = (uint32_t)&interrupt_isr008 >> 16;
	
	interrupt_IDT[14].offset1 = (uint32_t)&interrupt_isr014 & 0xFFFF;
	interrupt_IDT[14].selector = cs;
	interrupt_IDT[14].ignore = 0;
	interrupt_IDT[14].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[14].offset2 = (uint32_t)&interrupt_isr014 >> 16;

	interrupt_IDT[32].offset1 = (uint32_t)&interrupt_irq032 & 0xFFFF;
	interrupt_IDT[32].selector = cs;
	interrupt_IDT[32].ignore = 0;
	interrupt_IDT[32].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[32].offset2 = (uint32_t)&interrupt_irq032 >> 16;
	
	interrupt_IDT[33].offset1 = (uint32_t)&interrupt_irq033 & 0xFFFF;
	interrupt_IDT[33].selector = cs;
	interrupt_IDT[33].ignore = 0;
	interrupt_IDT[33].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[33].offset2 = (uint32_t)&interrupt_irq033 >> 16;

	interrupt_IDT[34].offset1 = (uint32_t)&interrupt_irq034 & 0xFFFF;
	interrupt_IDT[34].selector = cs;
	interrupt_IDT[34].ignore = 0;
	interrupt_IDT[34].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[34].offset2 = (uint32_t)&interrupt_irq034 >> 16;

	interrupt_IDT[35].offset1 = (uint32_t)&interrupt_irq035 & 0xFFFF;
	interrupt_IDT[35].selector = cs;
	interrupt_IDT[35].ignore = 0;
	interrupt_IDT[35].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[35].offset2 = (uint32_t)&interrupt_irq035 >> 16;

	interrupt_IDT[36].offset1 = (uint32_t)&interrupt_irq036 & 0xFFFF;
	interrupt_IDT[36].selector = cs;
	interrupt_IDT[36].ignore = 0;
	interrupt_IDT[36].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[36].offset2 = (uint32_t)&interrupt_irq036 >> 16;

	interrupt_IDT[37].offset1 = (uint32_t)&interrupt_irq037 & 0xFFFF;
	interrupt_IDT[37].selector = cs;
	interrupt_IDT[37].ignore = 0;
	interrupt_IDT[37].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[37].offset2 = (uint32_t)&interrupt_irq037 >> 16;

	interrupt_IDT[38].offset1 = (uint32_t)&interrupt_irq038 & 0xFFFF;
	interrupt_IDT[38].selector = cs;
	interrupt_IDT[38].ignore = 0;
	interrupt_IDT[38].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[38].offset2 = (uint32_t)&interrupt_irq038 >> 16;

	interrupt_IDT[39].offset1 = (uint32_t)&interrupt_irq039 & 0xFFFF;
	interrupt_IDT[39].selector = cs;
	interrupt_IDT[39].ignore = 0;
	interrupt_IDT[39].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[39].offset2 = (uint32_t)&interrupt_irq039 >> 16;

	interrupt_IDT[40].offset1 = (uint32_t)&interrupt_irq040 & 0xFFFF;
	interrupt_IDT[40].selector = cs;
	interrupt_IDT[40].ignore = 0;
	interrupt_IDT[40].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[40].offset2 = (uint32_t)&interrupt_irq040 >> 16;

	interrupt_IDT[41].offset1 = (uint32_t)&interrupt_irq041 & 0xFFFF;
	interrupt_IDT[41].selector = cs;
	interrupt_IDT[41].ignore = 0;
	interrupt_IDT[41].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[41].offset2 = (uint32_t)&interrupt_irq041 >> 16;

	interrupt_IDT[42].offset1 = (uint32_t)&interrupt_irq042 & 0xFFFF;
	interrupt_IDT[42].selector = cs;
	interrupt_IDT[42].ignore = 0;
	interrupt_IDT[42].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[42].offset2 = (uint32_t)&interrupt_irq042 >> 16;

	interrupt_IDT[43].offset1 = (uint32_t)&interrupt_irq043 & 0xFFFF;
	interrupt_IDT[43].selector = cs;
	interrupt_IDT[43].ignore = 0;
	interrupt_IDT[43].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[43].offset2 = (uint32_t)&interrupt_irq043 >> 16;

	interrupt_IDT[44].offset1 = (uint32_t)&interrupt_irq044 & 0xFFFF;
	interrupt_IDT[44].selector = cs;
	interrupt_IDT[44].ignore = 0;
	interrupt_IDT[44].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[44].offset2 = (uint32_t)&interrupt_irq044 >> 16;

	interrupt_IDT[45].offset1 = (uint32_t)&interrupt_irq045 & 0xFFFF;
	interrupt_IDT[45].selector = cs;
	interrupt_IDT[45].ignore = 0;
	interrupt_IDT[45].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[45].offset2 = (uint32_t)&interrupt_irq045 >> 16;

	interrupt_IDT[46].offset1 = (uint32_t)&interrupt_irq046 & 0xFFFF;
	interrupt_IDT[46].selector = cs;
	interrupt_IDT[46].ignore = 0;
	interrupt_IDT[46].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[46].offset2 = (uint32_t)&interrupt_irq046 >> 16;

	interrupt_IDT[47].offset1 = (uint32_t)&interrupt_irq047 & 0xFFFF;
	interrupt_IDT[47].selector = cs;
	interrupt_IDT[47].ignore = 0;
	interrupt_IDT[47].flags = IDT_FLAGS_INTERRUPT;
	interrupt_IDT[47].offset2 = (uint32_t)&interrupt_irq047 >> 16;
	
	// Load IDTR into CPU
	asm volatile (
		"lidt %0"
		:
		: "m"(interrupt_IDTR)
	);
}

void interrupt_pic_init(uint8_t offset1, uint8_t offset2) {
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	// outb(PIC1_DATA, a1);   // restore saved masks.
	// outb(PIC2_DATA, a2);
	// Enable all interrupts
	outb(PIC1_DATA, 0x00);
	outb(PIC2_DATA, 0x00);
	
}

void interrupt_enable() {
	asm ("sti");
}

void interrupt_disable() {
	asm ("cli");
}

void interrupt_pic_end(uint8_t irq) {
	if (irq >= 40) {
		outb(PIC2_COMMAND, PIC_EOI);
	}
	outb(PIC1_COMMAND, PIC_EOI);
}

bool interrupt_init(uint16_t cs) {
	interrupt_idt_init(cs);
	interrupt_pic_init(32, 40);
	if (pit_init()) {
		println("Error Initializing PIT");
		return true;
	}
	interrupt_disable();
	interrupt_enable();
	return false;
}

__attribute__ ((interrupt)) void interrupt_isr000(interruptFrame_t* interruptFrame) {
	println("Interrupt: 000, Divide by zero fault!");
	interrupt_Counter[0] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr001(interruptFrame_t* interruptFrame) {
	println("Interrupt: 001, Debug");
	interrupt_Counter[1] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr002(interruptFrame_t* interruptFrame) {
	println("Interrupt: 002, Non-maskable Interrupt");
	interrupt_Counter[2] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr008(interruptFrame_t* interruptFrame) {
	println("Interrupt: 008, Double fault!");
	interrupt_Counter[8] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr014(interruptFrame_t* interruptFrame) {
	println("Interrupt: 014, Page Fault!");
	interrupt_Counter[14] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq032(interruptFrame_t* interruptFrame) {
	interrupt_disable();
	interrupt_Counter[32] += 1;

	// time keeping
	pit_increment_time();

	
	interrupt_pic_end(32);
	interrupt_enable();
}

__attribute__ ((interrupt)) void interrupt_irq033(interruptFrame_t* interruptFrame) {
	// test interrupt
	println("Interrupt: 033");
	interrupt_Counter[33] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq034(interruptFrame_t* interruptFrame) {
	// test interrupt
	println("Interrupt: 034");
	interrupt_Counter[34] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq035(interruptFrame_t* interruptFrame) {
	// test interrupt
	println("Interrupt: 035");
	interrupt_Counter[35] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq036(interruptFrame_t* interruptFrame) {
	serial_interrupt_read();
	interrupt_Counter[35] += 1;
	interrupt_pic_end(36);

}

__attribute__ ((interrupt)) void interrupt_irq037(interruptFrame_t* interruptFrame) {
	// test interrupt
	println("Interrupt: 037");
	interrupt_Counter[37] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq038(interruptFrame_t* interruptFrame) {
	// test interrupt
	println("Interrupt: 038");
	interrupt_Counter[38] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq039(interruptFrame_t* interruptFrame) {
	// test interrupt
	println("Interrupt: 039");
	interrupt_Counter[39] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq040(interruptFrame_t* interruptFrame) {
	// test interrupt
	println("Interrupt: 040");
	interrupt_Counter[40] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq041(interruptFrame_t* interruptFrame) {
	// test interrupt
	println("Interrupt: 041");
	interrupt_Counter[41] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq042(interruptFrame_t* interruptFrame) {
	// test interrupt
	println("Interrupt: 042");
	interrupt_Counter[42] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq043(interruptFrame_t* interruptFrame) {
	// test interrupt
	println("Interrupt: 043");
	interrupt_Counter[43] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq044(interruptFrame_t* interruptFrame) {
	// test interrupt
	println("Interrupt: 044");
	interrupt_Counter[44] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq045(interruptFrame_t* interruptFrame) {
	// test interrupt
	println("Interrupt: 045");
	interrupt_Counter[45] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq046(interruptFrame_t* interruptFrame) {
	interrupt_disable();
	println("Interrupt: 046");
	interrupt_Counter[46] += 1;

	ata_read_sector();

	interrupt_pic_end(46);
	interrupt_enable();
}

__attribute__ ((interrupt)) void interrupt_irq047(interruptFrame_t* interruptFrame) {
	// test interrupt
	println("Interrupt: 047");
	interrupt_Counter[47] += 1;
}