/*
################################
#           [TatOS]            #
#         File: IDT.h          #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#ifndef IDTHander_H
#define IDTHander_H

#include <stdint.h>

// Not made by Tato
struct Registers {
    uint32_t ds;                             
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; 
    uint32_t int_no, error_code;                    
    uint32_t eip, cs, eflags, useresp, ss;          
};

struct IDTEntry {
    uint16_t base_low;   
    uint16_t selector;  
    uint8_t  zero;       
    uint8_t  flags;   
    uint16_t base_high;  
} __attribute__((packed));

struct IDTPtr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
typedef void (*InterruptHandler)(Registers* regs);

// Made by Tato
namespace TatOS
{
    namespace IDT
    {
        void Interruptions(bool Condition);

        namespace Interrupts 
        {
            void Initialize();
            void RegisterHandler(uint8_t vector, InterruptHandler handler);
        }
    }
}

#endif