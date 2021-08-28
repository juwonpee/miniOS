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



/* Declare constants for the multiboot header. */
.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */
 

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
 

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
  push %ecx
	push %eax
	push %ebx
	call kernel_init
	cli
1:	hlt
	jmp 1b
	
.size _start, . - _start