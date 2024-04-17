#include "kernel/interrupt.hpp"
#include "driver/io.hpp"
#include "driver/pit.hpp"
#include "driver/serial.hpp"


alignas(16) IDT_t ISRTable[256];
IDTR_t IDTR;
uint64_t interruptCounter[256];





void interrupt::init(uint16_t codeSegment) {
    IDTR.limit = 256 * sizeof(IDT_t) - 1;
    IDTR.base = (uint32_t)&ISRTable;

    // Initialize ISRTables
    for (int x = 0; x < 256; x++) {
        ISRTable[x].selector = codeSegment;
        ISRTable[x].zero = 0;
        ISRTable[x].present = false;
    }

    // Initialize PIC
    io::outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io::outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io::outb(PIC1_DATA, PIC1_OFFSET);                 // ICW2: Master PIC vector offset
	io::outb(PIC2_DATA, PIC2_OFFSET);                 // ICW2: Slave PIC vector offset
	io::outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io::outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io::outb(PIC1_DATA, ICW4_8086);
	io::outb(PIC2_DATA, ICW4_8086);
	// io::outb(PIC1_DATA, a1);   // restore saved masks.
	// io::outb(PIC2_DATA, a2);
	// Enable all interrupts
	io::outb(PIC1_DATA, 0x00);
	io::outb(PIC2_DATA, 0x00);


    // register ISRs
    registerISR(0, isr000_DE, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr001_DB, IDT_FLAGS_TRAP);
    registerISR(0, isr002_NMI, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr003_BP, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr004_OF, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr005_BR, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr006_UD, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr007_NM, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr008_DF, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr009_MF, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr010_TS, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr011_NP, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr012_SS, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr013_GP, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr014_PF, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr016_MF, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr017_AC, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr018_MC, IDT_FLAGS_INTERRUPT);
    registerISR(0, isr019_XM, IDT_FLAGS_INTERRUPT);
    
    // Enable pit
    pit::pitInit();
};

void interrupt::registerISR(uint8_t index, void(*isr)(IDTInterruptFrame_t* interruptFrame), uint8_t flags) {
    ISRTable[index].offset_1 = (uint32_t)&isr & 0xFFFF;
    ISRTable[index].offset_2 = (uint32_t)&isr >> 16;
    ISRTable[index].flags = flags;
    ISRTable[index].present = true;
}

void interrupt::registerISR(uint8_t index, void(*isr)(IDTInterruptFrame_t* interruptFrame, uint32_t errorCode), uint8_t flags) {
    ISRTable[index].offset_1 = (uint32_t)&isr & 0xFFFF;
    ISRTable[index].offset_2 = (uint32_t)&isr >> 16;
    ISRTable[index].flags = flags;
    ISRTable[index].present = true;
}

void interrupt::enable() {
    asm("sti");
};

void interrupt::disable() {
    asm("cli");
}

void interrupt::picReturn(uint8_t irq) {
    if (irq >= 40) {
		io::outb(PIC2_COMMAND, PIC_EOI);
	}
	io::outb(PIC1_COMMAND, PIC_EOI);
}

void interrupt::isr_reserved(IDTInterruptFrame_t* interruptFrame) {
}

void interrupt::isr000_DE(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[0]++;
}

void interrupt::isr001_DB(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[1]++;
}
void interrupt::isr002_NMI(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[2]++;
}

void interrupt::isr003_BP(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[3]++;
}

void interrupt::isr004_OF(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[4]++;
}

void interrupt::isr005_BR(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[5]++;
}

void interrupt::isr006_UD(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[6]++;
}

void interrupt::isr007_NM(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[7]++;
}

void interrupt::isr008_DF(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[8]++;
}

void interrupt::isr009_MF(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[9]++;
}

void interrupt::isr010_TS(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[10]++;
}

void interrupt::isr011_NP(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[11]++;
}

void interrupt::isr012_SS(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[12]++;
}

void interrupt::isr013_GP(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[13]++;
}

void interrupt::isr014_PF(IDTInterruptFrame_t* interruptFrame, uint32_t errorCode) {
    interruptCounter[14]++;
}
// #15 is reserved
// void interrupt::isr002_NMI(IDTInterruptFrame_t* interruptFrame) {
//     interruptCounter[2]++;
// }

void interrupt::isr016_MF(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[16]++;
}

void interrupt::isr017_AC(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[17]++;
}

void interrupt::isr018_MC(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[18]++;
}

void interrupt::isr019_XM(IDTInterruptFrame_t* interruptFrame) {
    interruptCounter[19]++;
}

// reserved 20~31