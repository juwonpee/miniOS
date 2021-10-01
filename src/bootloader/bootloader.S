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

#define ASM_FILE        1
#include "multiboot2.h"

/*  C symbol format. HAVE_ASM_USCORE is defined by configure. */
#ifdef HAVE_ASM_USCORE
# define EXT_C(sym)                     _ ## sym
#else
# define EXT_C(sym)                     sym
#endif

.section .multiboot2
.align 8
multiboot2_header_start:
	/*  magic */
	.long   MULTIBOOT2_HEADER_MAGIC
	/*  ISA: i386 */
	.long   MULTIBOOT_ARCHITECTURE_I386
	/*  Header length. */
	.long   multiboot2_header_end - multiboot2_header_start
	/*  checksum */
	.long   -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (multiboot2_header_end - multiboot2_header_start))

	.short 	MULTIBOOT_HEADER_TAG_END
	.short 	0
	.long 	8
multiboot2_header_end:

.section .bss
.align 16
stack_bottom:
.skip 0x1000000 # 16 MiB
stack_top:
 

.section .text
.global _start
.type _start, @function
_start:
	# setup stack
	mov $stack_top, %esp
	mov %esp, %ebp

	# push parameters to kernel init function
	mov $_end, %ecx
	push %ecx
	push %eax						/* Pointer to multiboot information structure */						
	push %ebx						/* MAGIC value */
	call kernel_init
	cli
		hlt
	
.size _start, . - _start
