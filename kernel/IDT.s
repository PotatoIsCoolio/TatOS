; I did not make any of this really. Just tiny adjustments
[bits 32]
global load_idt
global isr_stub_table
extern isr_handler

; Macro for interrupts that DO NOT push an error code automatically
%macro ISR_NOERR 1
global isr_stub_%1
isr_stub_%1:
    push dword 0    ; Dummy error code
    push dword %1   ; Vector number
    jmp isr_common
%endmacro

; Macro for CPU exceptions that DO push an error code automatically
%macro ISR_ERR 1
global isr_stub_%1
isr_stub_%1:
    push dword %1   ; Vector number (error code already on stack)
    jmp isr_common
%endmacro

; CPU Exceptions (0 - 31)
ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8     ; Double Fault (#DF) pushes error code
ISR_NOERR 9
ISR_ERR   10    ; Invalid TSS (#TS) pushes error code
ISR_ERR   11    ; Segment Not Present (#NP) pushes error code
ISR_ERR   12    ; Stack Fault (#SS) pushes error code
ISR_ERR   13    ; GP Fault (#GP) pushes error code
ISR_ERR   14    ; Page Fault (#PF) pushes error code
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR   17    ; Alignment Check pushes error code
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_ERR   21    ; Control Protection pushes error code
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_ERR   29
ISR_ERR   30    ; Security Exception pushes error code
ISR_NOERR 31

; Hardware IRQs (32 - 47)
%assign i 32
%rep 16
ISR_NOERR i
%assign i i+1
%endrep

; Common interrupt handler entry
isr_common:
    pusha               ; Pushes EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX

    mov ax, ds
    push eax            ; Save Data Segment selector

    mov ax, 0x10        ; Load Kernel Data Segment (0x10 in standard GDT)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp            ; Pass pointer to Registers struct as argument to isr_handler
    call isr_handler
    add esp, 4          ; Clean up argument

    pop eax             ; Restore original Data Segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                ; Restore general registers
    add esp, 8     
    iret     

; Load IDT Register
load_idt:
    mov eax, [esp + 4]  ; Address of IDTPtr struct
    lidt [eax]
    ret

; Lookup table for C++ initialization loop
section .data
isr_stub_table:
%assign i 0
%rep 48
    dd isr_stub_%+i
%assign i i+1
%endrep