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
	
	interrupt_IDT[0].offset1 = (uint32_t)&interrupt_isr000 & 0xFFFF;
	interrupt_IDT[0].selector = cs;
	interrupt_IDT[0].ignore = 0;
	interrupt_IDT[0].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[0].offset2 = (uint32_t)&interrupt_isr000 >> 16;
	
	interrupt_IDT[1].offset1 = (uint32_t)&interrupt_isr001 & 0xFFFF;
	interrupt_IDT[1].selector = cs;
	interrupt_IDT[1].ignore = 0;
	interrupt_IDT[1].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[1].offset2 = (uint32_t)&interrupt_isr001 >> 16;
	
	interrupt_IDT[2].offset1 = (uint32_t)&interrupt_isr002 & 0xFFFF;
	interrupt_IDT[2].selector = cs;
	interrupt_IDT[2].ignore = 0;
	interrupt_IDT[2].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[2].offset2 = (uint32_t)&interrupt_isr002 >> 16;
	
	interrupt_IDT[3].offset1 = (uint32_t)&interrupt_isr003 & 0xFFFF;
	interrupt_IDT[3].selector = cs;
	interrupt_IDT[3].ignore = 0;
	interrupt_IDT[3].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[3].offset2 = (uint32_t)&interrupt_isr003 >> 16;
	
	interrupt_IDT[4].offset1 = (uint32_t)&interrupt_isr004 & 0xFFFF;
	interrupt_IDT[4].selector = cs;
	interrupt_IDT[4].ignore = 0;
	interrupt_IDT[4].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[4].offset2 = (uint32_t)&interrupt_isr004 >> 16;
	
	interrupt_IDT[5].offset1 = (uint32_t)&interrupt_isr005 & 0xFFFF;
	interrupt_IDT[5].selector = cs;
	interrupt_IDT[5].ignore = 0;
	interrupt_IDT[5].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[5].offset2 = (uint32_t)&interrupt_isr005 >> 16;
	
	interrupt_IDT[6].offset1 = (uint32_t)&interrupt_isr006 & 0xFFFF;
	interrupt_IDT[6].selector = cs;
	interrupt_IDT[6].ignore = 0;
	interrupt_IDT[6].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[6].offset2 = (uint32_t)&interrupt_isr006 >> 16;	

	interrupt_IDT[7].offset1 = (uint32_t)&interrupt_isr007 & 0xFFFF;
	interrupt_IDT[7].selector = cs;
	interrupt_IDT[7].ignore = 0;
	interrupt_IDT[7].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[7].offset2 = (uint32_t)&interrupt_isr007 >> 16;

	interrupt_IDT[8].offset1 = (uint32_t)&interrupt_isr008 & 0xFFFF;
	interrupt_IDT[8].selector = cs;
	interrupt_IDT[8].ignore = 0;
	interrupt_IDT[8].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[8].offset2 = (uint32_t)&interrupt_isr008 >> 16;
	
	interrupt_IDT[9].offset1 = (uint32_t)&interrupt_isr009 & 0xFFFF;
	interrupt_IDT[9].selector = cs;
	interrupt_IDT[9].ignore = 0;
	interrupt_IDT[9].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[9].offset2 = (uint32_t)&interrupt_isr009 >> 16;
	
	interrupt_IDT[10].offset1 = (uint32_t)&interrupt_isr010 & 0xFFFF;
	interrupt_IDT[10].selector = cs;
	interrupt_IDT[10].ignore = 0;
	interrupt_IDT[10].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[10].offset2 = (uint32_t)&interrupt_isr010 >> 16;
	
	interrupt_IDT[11].offset1 = (uint32_t)&interrupt_isr011 & 0xFFFF;
	interrupt_IDT[11].selector = cs;
	interrupt_IDT[11].ignore = 0;
	interrupt_IDT[11].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[11].offset2 = (uint32_t)&interrupt_isr011 >> 16;
	
	interrupt_IDT[12].offset1 = (uint32_t)&interrupt_isr012 & 0xFFFF;
	interrupt_IDT[12].selector = cs;
	interrupt_IDT[12].ignore = 0;
	interrupt_IDT[12].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[12].offset2 = (uint32_t)&interrupt_isr012 >> 16;
	
	interrupt_IDT[13].offset1 = (uint32_t)&interrupt_isr013 & 0xFFFF;
	interrupt_IDT[13].selector = cs;
	interrupt_IDT[13].ignore = 0;
	interrupt_IDT[13].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[13].offset2 = (uint32_t)&interrupt_isr013 >> 16;
	
	interrupt_IDT[14].offset1 = (uint32_t)&interrupt_isr014 & 0xFFFF;
	interrupt_IDT[14].selector = cs;
	interrupt_IDT[14].ignore = 0;
	interrupt_IDT[14].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[14].offset2 = (uint32_t)&interrupt_isr014 >> 16;
	
	interrupt_IDT[15].offset1 = (uint32_t)&interrupt_isr015 & 0xFFFF;
	interrupt_IDT[15].selector = cs;
	interrupt_IDT[15].ignore = 0;
	interrupt_IDT[15].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[15].offset2 = (uint32_t)&interrupt_isr015 >> 16;
	
	interrupt_IDT[16].offset1 = (uint32_t)&interrupt_isr016 & 0xFFFF;
	interrupt_IDT[16].selector = cs;
	interrupt_IDT[16].ignore = 0;
	interrupt_IDT[16].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[16].offset2 = (uint32_t)&interrupt_isr016 >> 16;
	
	interrupt_IDT[17].offset1 = (uint32_t)&interrupt_isr017 & 0xFFFF;
	interrupt_IDT[17].selector = cs;
	interrupt_IDT[17].ignore = 0;
	interrupt_IDT[17].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[17].offset2 = (uint32_t)&interrupt_isr017 >> 16;
	
	interrupt_IDT[18].offset1 = (uint32_t)&interrupt_isr018 & 0xFFFF;
	interrupt_IDT[18].selector = cs;
	interrupt_IDT[18].ignore = 0;
	interrupt_IDT[18].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[18].offset2 = (uint32_t)&interrupt_isr018 >> 16;
	
	interrupt_IDT[19].offset1 = (uint32_t)&interrupt_isr019 & 0xFFFF;
	interrupt_IDT[19].selector = cs;
	interrupt_IDT[19].ignore = 0;
	interrupt_IDT[19].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[19].offset2 = (uint32_t)&interrupt_isr019 >> 16;
	
	interrupt_IDT[20].offset1 = (uint32_t)&interrupt_isr020 & 0xFFFF;
	interrupt_IDT[20].selector = cs;
	interrupt_IDT[20].ignore = 0;
	interrupt_IDT[20].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[20].offset2 = (uint32_t)&interrupt_isr020 >> 16;
	
	interrupt_IDT[21].offset1 = (uint32_t)&interrupt_isr021 & 0xFFFF;
	interrupt_IDT[21].selector = cs;
	interrupt_IDT[21].ignore = 0;
	interrupt_IDT[21].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[21].offset2 = (uint32_t)&interrupt_isr021 >> 16;
	
	interrupt_IDT[22].offset1 = (uint32_t)&interrupt_isr022 & 0xFFFF;
	interrupt_IDT[22].selector = cs;
	interrupt_IDT[22].ignore = 0;
	interrupt_IDT[22].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[22].offset2 = (uint32_t)&interrupt_isr022 >> 16;
	
	interrupt_IDT[23].offset1 = (uint32_t)&interrupt_isr023 & 0xFFFF;
	interrupt_IDT[23].selector = cs;
	interrupt_IDT[23].ignore = 0;
	interrupt_IDT[23].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[23].offset2 = (uint32_t)&interrupt_isr023 >> 16;
	
	interrupt_IDT[24].offset1 = (uint32_t)&interrupt_isr024 & 0xFFFF;
	interrupt_IDT[24].selector = cs;
	interrupt_IDT[24].ignore = 0;
	interrupt_IDT[24].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[24].offset2 = (uint32_t)&interrupt_isr024 >> 16;
	
	interrupt_IDT[25].offset1 = (uint32_t)&interrupt_isr025 & 0xFFFF;
	interrupt_IDT[25].selector = cs;
	interrupt_IDT[25].ignore = 0;
	interrupt_IDT[25].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[25].offset2 = (uint32_t)&interrupt_isr025 >> 16;
	
	interrupt_IDT[26].offset1 = (uint32_t)&interrupt_isr026 & 0xFFFF;
	interrupt_IDT[26].selector = cs;
	interrupt_IDT[26].ignore = 0;
	interrupt_IDT[26].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[26].offset2 = (uint32_t)&interrupt_isr026 >> 16;
	
	interrupt_IDT[27].offset1 = (uint32_t)&interrupt_isr027 & 0xFFFF;
	interrupt_IDT[27].selector = cs;
	interrupt_IDT[27].ignore = 0;
	interrupt_IDT[27].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[27].offset2 = (uint32_t)&interrupt_isr027 >> 16;
	
	interrupt_IDT[28].offset1 = (uint32_t)&interrupt_isr028 & 0xFFFF;
	interrupt_IDT[28].selector = cs;
	interrupt_IDT[28].ignore = 0;
	interrupt_IDT[28].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[28].offset2 = (uint32_t)&interrupt_isr028 >> 16;
	
	interrupt_IDT[29].offset1 = (uint32_t)&interrupt_isr029 & 0xFFFF;
	interrupt_IDT[29].selector = cs;
	interrupt_IDT[29].ignore = 0;
	interrupt_IDT[29].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[29].offset2 = (uint32_t)&interrupt_isr029 >> 16;
	
	interrupt_IDT[30].offset1 = (uint32_t)&interrupt_isr030 & 0xFFFF;
	interrupt_IDT[30].selector = cs;
	interrupt_IDT[30].ignore = 0;
	interrupt_IDT[30].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[30].offset2 = (uint32_t)&interrupt_isr030 >> 16;
	
	interrupt_IDT[31].offset1 = (uint32_t)&interrupt_isr031 & 0xFFFF;
	interrupt_IDT[31].selector = cs;
	interrupt_IDT[31].ignore = 0;
	interrupt_IDT[31].flags = IDT_FLAGS_TRAP;
	interrupt_IDT[31].offset2 = (uint32_t)&interrupt_isr031 >> 16;

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

__attribute__ ((interrupt)) void interrupt_isr000(IDT_interruptFrame_t* interruptFrame) {
	println("Interrupt: 000, Divide by zero fault!");
	interrupt_Counter[0] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr001(IDT_interruptFrame_t* interruptFrame) {
	println("Interrupt: 001, Debug");
	interrupt_Counter[1] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr002(IDT_interruptFrame_t* interruptFrame) {
	println("Interrupt: 002, Non-maskable Interrupt");
	interrupt_Counter[2] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr003(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 003:, Breakpoint");
	interrupt_Counter[3] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr004(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 004, Overflow");
	interrupt_Counter[4] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr005(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 005, Bound range exceeded");
	interrupt_Counter[5] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr006(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 006, Invalid Opcode");
	interrupt_Counter[6] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr007(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 007, Device not available");
	interrupt_Counter[7] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr008(IDT_interruptFrame_t* interruptFrame) {
	println("Interrupt: 008, Double fault!");
	interrupt_Counter[8] += 1;
	panic();
}

__attribute__ ((interrupt)) void interrupt_isr009(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 009, Coprocessor segment overrun");
	interrupt_Counter[9] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr010(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 010, Invalid TSS");
	interrupt_Counter[10] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr011(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 011, Segment not present");
	interrupt_Counter[11] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr012(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 012, Stack-Segment fault");
	interrupt_Counter[12] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr013(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 013, General protection fault");
	interrupt_Counter[13] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr014(IDT_interruptFrame_t* interruptFrame, uint32_t errorCode) {
	println("Interrupt: 014, Page Fault!");
	char tempString[64];

	uint32_t* cr2;
	asm volatile (
        "movl %%cr2, %0" 
        : "=a" (cr2)
        : 
    );
	IDT_pageFault_error_t pageFault_error;
	pageFault_error.data = errorCode;


	print("Fault occured at: ");
	println(itoa((uint32_t)cr2, tempString, 16));

	// Call page fault handler
	memory_interrupt_handler(pageFault_error, (void*)cr2);


	interrupt_Counter[14] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr015(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 015, Intel reserved");
	interrupt_Counter[15] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr016(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 016, x87 Floating point exception");
	interrupt_Counter[16] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr017(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 017, Alignment check");
	interrupt_Counter[17] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr018(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 018, Machine check");
	interrupt_Counter[18] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr019(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 019, SIMD floating point exception");
	interrupt_Counter[18] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr020(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 020, Virtualization exception");
	interrupt_Counter[18] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr021(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 021, Intel reserved");
	interrupt_Counter[18] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr022(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 022, Intel reserved");
	interrupt_Counter[18] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr023(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 023, Intel reserved");
	interrupt_Counter[18] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr024(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 024, Intel reserved");
	interrupt_Counter[18] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr025(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 025, Intel reserved");
	interrupt_Counter[18] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr026(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 026, Intel reserved");
	interrupt_Counter[18] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr027(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 027, Intel reserved");
	interrupt_Counter[18] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr028(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 028, Intel reserved");
	interrupt_Counter[18] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr029(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 029, Intel reserved");
	interrupt_Counter[18] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr030(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 030, Security Exception");
	interrupt_Counter[18] += 1;
}

__attribute__ ((interrupt)) void interrupt_isr031(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 031, Intel reserved");
	interrupt_Counter[18] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq032(IDT_interruptFrame_t* interruptFrame) {
	interrupt_disable();
	interrupt_Counter[32] += 1;

	// time keeping
	pit_increment_time();

	
	interrupt_pic_end(32);
	interrupt_enable();
}

__attribute__ ((interrupt)) void interrupt_irq033(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 033");
	interrupt_Counter[33] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq034(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 034");
	interrupt_Counter[34] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq035(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 035");
	interrupt_Counter[35] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq036(IDT_interruptFrame_t* interruptFrame) {
	serial_interrupt_read();
	interrupt_Counter[35] += 1;
	interrupt_pic_end(36);
}

__attribute__ ((interrupt)) void interrupt_irq037(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 037");
	interrupt_Counter[37] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq038(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 038");
	interrupt_Counter[38] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq039(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 039");
	interrupt_Counter[39] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq040(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 040");
	interrupt_Counter[40] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq041(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 041");
	interrupt_Counter[41] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq042(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 042");
	interrupt_Counter[42] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq043(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 043");
	interrupt_Counter[43] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq044(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 044");
	interrupt_Counter[44] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq045(IDT_interruptFrame_t* interruptFrame) {
	
	println("Interrupt: 045");
	interrupt_Counter[45] += 1;
}

__attribute__ ((interrupt)) void interrupt_irq046(IDT_interruptFrame_t* interruptFrame) {
	interrupt_disable();
	println("Interrupt: 046");
	interrupt_Counter[46] += 1;

	ata_primary_read_sector();

	interrupt_pic_end(46);
	interrupt_enable();
}

__attribute__ ((interrupt)) void interrupt_irq047(IDT_interruptFrame_t* interruptFrame) {
	interrupt_disable();
	println("Interrupt: 047");
	interrupt_Counter[47] += 1;

	ata_secondary_read_sector();
	
	interrupt_pic_end(47);
	interrupt_enable();
}