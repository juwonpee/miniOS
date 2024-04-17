#pragma once
#include "include/stdint-gcc.hpp"

#define PIC1_OFFSET				0x20								/* IO base address for master PIC */
#define PIC2_OFFSET				0xA0								/* IO base address for slave PIC */
#define PIC1_COMMAND			PIC1_OFFSET
#define PIC1_DATA				(PIC1_OFFSET+1)
#define PIC2_COMMAND			PIC2_OFFSET
#define PIC2_DATA				(PIC2_OFFSET+1)
#define PIC_EOI					0x20

#define ICW1_ICW4				0x01								/* ICW4 (not) needed */
#define ICW1_SINGLE				0x02								/* Single (cascade) mode */
#define ICW1_INTERVAL4			0x04								/* Call address interval 4 (8) */
#define ICW1_LEVEL				0x08								/* Level triggered (edge) mode */
#define ICW1_INIT				0x10								/* Initialization - required! */
 
#define ICW4_8086				0x01								/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO				0x02								/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE			0x08								/* Buffered mode/slave */
#define ICW4_BUF_MASTER			0x0C								/* Buffered mode/master */
#define ICW4_SFNM				0x10								/* Special fully nested (not) */

#define IDT_FLAGS_TASK			0b10000101
#define IDT_FLAGS_INTERRUPT 	0b10001110
#define IDT_FLAGS_TRAP			0b10001111

typedef struct IDT_t {
    union {
        uint64_t data;
        struct {
            uint16_t offset_1;        // offset bits 0..15
            uint16_t selector;        // a code segment selector in GDT or LDT
            uint8_t  zero;            // unused, set to 0
			union {
				uint8_t flagsData;
				struct {
					uint8_t gateType:4;
					uint8_t DPL:2;
					uint8_t present:1;
					uint8_t flagsZero:1;
				};
			};
            uint8_t  flags; // gate type, dpl, and p fields
            uint16_t offset_2;        // offset bits 16..31
        } __attribute__ ((packed));
    };
} __attribute__ ((packed)) IDT_t;

typedef struct IDTR_t {
	uint16_t limit;
	uint32_t base;
} __attribute__ ((packed)) IDTR_t;

typedef struct IDTSelectorError_t {
	union {
		uint32_t data;
		struct {
			uint32_t E:1;																		// External				When set, the exception originated externally to the processor.
			uint32_t TBL:2;																		/* Table selector		0b00	The Selector Index references a descriptor in the GDT.
																														0b01	The Selector Index references a descriptor in the IDT.
																														0b10	The Selector Index references a descriptor in the LDT.
																														0b11	The Selector Index references a descriptor in the IDT. */
			uint32_t index:13;																	// Selector index		The index in the GDT, IDT or LDT. 
			uint32_t reserved:16;
		} __attribute__ ((packed));
	};
} __attribute__ ((packed)) IDTSelectorError_t;

typedef struct IDTInterruptFrame_t {
	uint32_t rsp;
} IDTInterruptFrame_t;

class interrupt {
	public:
	static void init(uint16_t codeSegment);
	static void registerISR(uint8_t index, void(*isr)(IDTInterruptFrame_t* interruptFrame), uint8_t flags);
	static void registerISR(uint8_t index, void(*isr)(IDTInterruptFrame_t* interruptFrame, uint32_t errorCode), uint8_t flags);
	static void enable();
	static void disable();

	static void picReturn(uint8_t irq);

	private:
	__attribute__ ((interrupt)) static void isr_reserved(IDTInterruptFrame_t* interruptFrame);									// For processor reserved/null ISRs
	__attribute__ ((interrupt)) static void isr000_DE(IDTInterruptFrame_t* interruptFrame); 									// Divide by zero
	__attribute__ ((interrupt)) static void isr001_DB(IDTInterruptFrame_t* interruptFrame);										// Debug
	__attribute__ ((interrupt)) static void isr002_NMI(IDTInterruptFrame_t* interruptFrame);									// Non maskable interrupt
	__attribute__ ((interrupt)) static void isr003_BP(IDTInterruptFrame_t* interruptFrame);										// Breakpoint
	__attribute__ ((interrupt)) static void isr004_OF(IDTInterruptFrame_t* interruptFrame);										// Overflow
	__attribute__ ((interrupt)) static void isr005_BR(IDTInterruptFrame_t* interruptFrame);										// Bound range exceeded
	__attribute__ ((interrupt)) static void isr006_UD(IDTInterruptFrame_t* interruptFrame);										// Undefined opcode
	__attribute__ ((interrupt)) static void isr007_NM(IDTInterruptFrame_t* interruptFrame);										// Coprocessor not detected
	__attribute__ ((interrupt)) static void isr008_DF(IDTInterruptFrame_t* interruptFrame);										// Double fault
	__attribute__ ((interrupt)) static void isr009_MF(IDTInterruptFrame_t* interruptFrame);										// Coprocessor segment overrun
	__attribute__ ((interrupt)) static void isr010_TS(IDTInterruptFrame_t* interruptFrame);										// Invalid TSS
	__attribute__ ((interrupt)) static void isr011_NP(IDTInterruptFrame_t* interruptFrame);										// Segment not present
	__attribute__ ((interrupt)) static void isr012_SS(IDTInterruptFrame_t* interruptFrame);										// Stack segmentation fault
	__attribute__ ((interrupt)) static void isr013_GP(IDTInterruptFrame_t* interruptFrame);										// General protection fault
	__attribute__ ((interrupt)) static void isr014_PF(IDTInterruptFrame_t* interruptFrame, uint32_t errorCode);					// Page fault
	// Reserved
	__attribute__ ((interrupt)) static void isr016_MF(IDTInterruptFrame_t* interruptFrame);										// Floating point math error
	__attribute__ ((interrupt)) static void isr017_AC(IDTInterruptFrame_t* interruptFrame);										// Alignment check
	__attribute__ ((interrupt)) static void isr018_MC(IDTInterruptFrame_t* interruptFrame);										// Machine check
	__attribute__ ((interrupt)) static void isr019_XM(IDTInterruptFrame_t* interruptFrame);										// SIMD exception
	// Reserved 20~31

	// from 32~255 user defined
};