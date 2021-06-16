[BITS 32]

GRUB_MAGIC_NUMBER       equ 0x1BADB002
GRUB_FLAGS              equ 0x00000003
GRUB_HEADER_CHECKSUM    equ -(GRUB_MAGIC_NUMBER + GRUB_FLAGS)


section .multiboot_header
align 4
header_start:
    dd GRUB_MAGIC_NUMBER                        ; magic number (multiboot 2)
    dd GRUB_FLAGS                               ; architecture 0 (protected mode i386)
    dd GRUB_HEADER_CHECKSUM                     ; header length

; Allocate 64KiB for stack
stack_bottom:
align 16
times 65536 db 0

section .bss
stack_top:


section .text

global start
start:
    mov esp, stack_top                          ; Set stack to top of memory
    
    mov eax, cr0
    and eax, 0x9FFFFFF
    mov cr0, eax

    push ebx

    extern init
    call init

    cli
    hlt