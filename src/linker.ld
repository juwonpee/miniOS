ENTRY(start)

SECTIONS {
    . = 1M;

    .multiboot_header.data : {
        *(.multiboot_header.data)
    }

    .multiboot_header.text : {
        *(.multiboot_header.text)
    }

    . = 64M;
    .text BLOCK(4K) : ALIGN(4K)
    {
        /* ensure that the multiboot header is at the beginning */
        *(.text)
    }
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(.COMMON)
        *(.bss)
    }

}