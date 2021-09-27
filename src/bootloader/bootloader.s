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

// Only the bootloader is written in GAS assembly as i copied it from osdev


.set MAGIC,    			0xE85250D6		/* 'magic number' lets bootloader find the header */
.set ARCHITECTURE,		0				/* 0 = i386 */
.set HEADER_LENGTH,		24
.set CHECKSUM,			-(MAGIC + ARCHITECTURE + HEADER_LENGTH)
.set TYPE0,				0
.set FLAGS0,			0
.set SIZE0,				8


.section .multiboot2
.align 8
.long MAGIC
.long ARCHITECTURE
.long HEADER_LENGTH
.long CHECKSUM
.word TYPE0
.word FLAGS0
.long SIZE0

/*.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM*/
 

.section .bss
.align 16
stack_bottom:
.skip 0x1000000 # 16 MiB
stack_top:
 

.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp
	mov $_end, %ecx
	mov %cs, %dx
	push %dx
	push %ecx
	push %eax						/* Pointer to multiboot information structure */						
	push %ebx						/* MAGIC value */
	call kernel_init
	cli
		hlt
	
.size _start, . - _start
