/*
################################
#           [TatOS]            #
#        File: IDT.cpp         #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "IDT.h"
#include "../HardwareManager/IO.h"
#include "../../Kernel.h"

// Ill clean up this messy ass code later. I didnt make most of this and its mostly pasted from other srcs I made a few things in here. I am sorry.

extern "C" {
    void load_idt(uint32_t idt_ptr_addr);
    extern uint32_t isr_stub_table[];
    void isr_handler(Registers* Registers);
}

struct GDTEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct GDTPtr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static GDTEntry gdt[3];
static GDTPtr   gdtr;

static void SetGDTGate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;
    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access      = access;
}

static void InstallGDT() {
    gdtr.limit = (sizeof(GDTEntry) * 3) - 1;
    gdtr.base  = (uint32_t)&gdt;

    SetGDTGate(0, 0, 0, 0, 0); // Null segment (0x00)
    SetGDTGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment (0x08)
    SetGDTGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment (0x10)

    asm volatile (
        "lgdt %0\n\t"
        "mov $0x10, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"
        "ljmp $0x08, $1f\n\t"
        "1:\n\t"
        : : "m"(gdtr) : "eax"
    );
}

static IDTEntry idt[256];
static IDTPtr   idtr;
static InterruptHandler InterruptHandlers[256] = { nullptr };

namespace TatOS
{
    namespace IDT
    {
        namespace Interrupts 
        {
            void SetGate(uint8_t Vector, uint32_t base, uint16_t selector, uint8_t flags) {
                idt[Vector].base_low  = base & 0xFFFF;
                idt[Vector].base_high = (base >> 16) & 0xFFFF;
                idt[Vector].selector  = selector;
                idt[Vector].zero      = 0;
                idt[Vector].flags     = flags;
            }

            void RemapPIC() 
            {
                outb(0x20, 0x11); io_wait();
                outb(0xA0, 0x11); io_wait();

                outb(0x21, 0x20); io_wait();
                outb(0xA1, 0x28); io_wait();

                outb(0x21, 0x04); io_wait();
                outb(0xA1, 0x02); io_wait();

                outb(0x21, 0x01); io_wait();
                outb(0xA1, 0x01); io_wait();

                outb(0x21, 0xFE); io_wait(); 
                outb(0xA1, 0xFF); io_wait();
            }

            void RegisterHandler(uint8_t Vector, InterruptHandler Handler) 
            {
                InterruptHandlers[Vector] = Handler;
                
                if (Vector >= 32 && Vector <= 39) 
                {
                    uint8_t mask = inb(0x21) & ~(1 << (Vector - 32));
                    outb(0x21, mask);
                } 
                else if (Vector >= 40 && Vector <= 47) 
                {
                    uint8_t mask = inb(0xA1) & ~(1 << (Vector - 40));
                    outb(0xA1, mask);
                }
            }

            void Initialize() 
            {
                InstallGDT();

                idtr.limit = (sizeof(IDTEntry) * 256) - 1;
                idtr.base  = (uint32_t)&idt;

                for (int i = 0; i < 256; i++) {
                    uint8_t stub_idx = (i < 48) ? i : 7;
                    SetGate(i, isr_stub_table[stub_idx], 0x08, 0x8E);
                }

                RemapPIC();
                load_idt((uint32_t)&idtr);
            }
        }
    }
}

extern "C" void isr_handler(Registers* Registers) 
{
    if (InterruptHandlers[Registers->int_no] != nullptr) 
    {
        InterruptHandlers[Registers->int_no](Registers);
    }

    if (Registers->int_no >= 32 && Registers->int_no <= 47) 
    {
        if (Registers->int_no >= 40) 
        {
            outb(0xA0, 0x20);
        }
        outb(0x20, 0x20);
    }
}

void TatOS::IDT::Interruptions(bool Condition)
{
    if (Condition) 
    {
        asm volatile("sti"); // Enables the interruptions
    }
    else // This is probably so unsafe and has such big risks. but we ball.
    {
        asm volatile("cli"); // Disables interruptions
    };
}