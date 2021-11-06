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
0~31:				CPU reserved interrupts
32~47:				PIC reserved interrupts
33:					Syscall Entry
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
#include "io.h"
#include "pit.h"
#include "string.h"
#include "memory.h"

#define PIC1					0x20								/* IO base address for master PIC */
#define PIC2					0xA0								/* IO base address for slave PIC */
#define PIC1_COMMAND			PIC1
#define PIC1_DATA				(PIC1+1)
#define PIC2_COMMAND			PIC2
#define PIC2_DATA				(PIC2+1)
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

void interrupt_idt_init(uint16_t cs);
void interrupt_pic_init(uint8_t offset1, uint8_t offset2);
void interrupt_disable();
void interrupt_enable();
bool interrupt_init(uint16_t cs);

void interrupt_pic_end(uint8_t irq);


__attribute__ ((interrupt)) void interrupt_isr000(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr001(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr002(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr003(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr004(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr005(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr006(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr007(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr008(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr009(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr010(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr011(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr012(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr013(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr014(IDT_interruptFrame_t* interruptFrame, uint32_t errorCode);
__attribute__ ((interrupt)) void interrupt_isr015(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr016(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr017(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr018(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr019(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr020(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr021(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr022(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr023(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr024(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr025(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr026(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr027(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr028(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr029(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr030(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_isr031(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq032(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq033(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq034(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq035(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq036(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq037(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq038(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq039(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq040(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq041(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq042(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq043(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq044(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq045(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq046(IDT_interruptFrame_t* interruptFrame);
__attribute__ ((interrupt)) void interrupt_irq047(IDT_interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq048(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq049(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq050(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq051(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq052(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq053(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq054(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq055(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq056(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq057(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq058(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq059(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq060(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq061(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq062(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq063(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq064(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq065(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq066(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq067(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq068(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq069(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq070(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq071(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq072(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq073(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq074(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq075(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq076(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq077(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq078(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq079(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq080(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq081(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq082(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq083(interruptFrame_t* interruptFrame);
// __attribute__ ((interrupt)) void interrupt_irq084(interruptFrame_t* interruptFrame);