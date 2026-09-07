OUTPUT_FORMAT("elf32-i386")
ENTRY(boot_entry)

SECTIONS {
    . = 0x00100000;

    .multiboot :
    {
        KEEP(*(.multiboot))
    }

    .text :
    {
        *(.text.entry)
        *(.text*)
    }

    .rodata : { *(.rodata*) }
    .data   : { *(.data*) }
    .bss    : { *(COMMON) *(.bss*) }
}