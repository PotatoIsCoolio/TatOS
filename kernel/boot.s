[bits 32]
section .text.entry

global boot_entry
extern KernelMain

boot_entry:
    cli ; Don't worry once I make the IDT we will enable interruptions.
    mov esp, stack_top

    push ebx
    push eax
    call KernelMain   

_stop:
    hlt
    jmp _stop

section .rodata
global _binary_initrd_tar_start
global _binary_initrd_tar_end

_binary_initrd_tar_start:
    incbin "iso/boot/initrd.tar"
_binary_initrd_tar_end:

; Allocate 16 KB for the kernel stack in .bss
section .bss
align 16
stack_bottom:
resb 16384 ; 16kb
stack_top:
