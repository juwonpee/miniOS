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

/*
Interrupts
0~31:			CPU reserved interrupts
32:				Syscall Entry
	eax		ebx		ecx											edx																				Explanation
EAX=0: File operations
	0		0		Pointer to file string (4096 byte length)	Return operation status															Request file locking & opening
																	0x0: Success
																	0x1: File does not exist
																	0x2: File is locked
																	0x3: File is unlockable (shared)

			1		Pointer to file string						Return file flags																Get file status
																	0b: Unlocked/Locked						
																	1b: Lockable/Unlockable					
																	2b: Permission to read
																	3b: Permission to write

			2		Pointer to file string						Return operation status															Request file unlocking
																	0x0: success							
																	0x1: failed								

			3		Pointer to file string						Return operation status															Request file creation
																	0x0: Success
																	0x1: File already exists
																	0x2: Illegal characters in filename
																	0x3: Filename too long
																	0x4: Directory does not exist
																	0x5: No permission to write in directory
			
			4		Pointer to file string						Return operation status															Request file deletion
																	0x0: Success
																	0x1: File is locked
																	0x2: File does not exist
																	0x3: No permission to write to file
																	0x4: No permission to write to directory

			5		Pointer to directory string 				Return operation status															Request directory creation
																	0x0: Success
																	0x1: Directory already exists
																	0x1: Subdirectory does not exist
																	0x2: No permission to write to directory				
			
			6 		Pointer to directory string					Return operation status																	Request directory deletion
																	0x0: Success
																	0x1: Directory does not exist
																	0x2: File/Files in directory are locked
																	0x3: No permission to write to directory	
EAX=1: 															
	4		Return PID																															Get PID
	5		Pointer to char								Return operation status						Return error code
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

#define IDT_FLAGS_TASK			0b10000101
#define IDT_FLAGS_INTERRUPT 	0b10001110
#define IDT_FLAGS_TRAP			0b10001111

typedef struct IDT_t {
	union {
		uint64_t data;
		struct {
			uint16_t offset1;
			uint16_t selector;
			uint8_t ignore;
			uint8_t flags; 
			uint16_t offset2:16;
		} __attribute ((packed));
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
__attribute__ ((interrupt)) void interrupt_isr008(interruptFrame_t* interruptFrame);
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
// __attribute__ ((interrupt)) void irq033(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq034(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq035(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq036(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq037(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq038(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq039(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq040(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq041(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq042(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq043(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq044(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq045(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq046(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq047(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq048(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq049(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq050(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq051(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq052(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq053(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq054(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq055(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq056(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq057(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq058(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq059(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq060(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq061(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq062(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq063(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq064(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq065(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq066(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq067(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq068(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq069(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq070(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq071(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq072(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq073(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq074(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq075(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq076(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq077(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq078(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq079(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq080(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq081(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq082(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq083(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void irq084(interruptFrame_t* interruptFrame);