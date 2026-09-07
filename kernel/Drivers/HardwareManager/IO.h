/*
################################
#           [TatOS]            #
#          File: IO.h          #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#ifndef IO_H
#define IO_H

#include <stdint.h>

static inline uint8_t inb(uint16_t port)
{
    uint8_t value;
    asm volatile ("inb %1, %0": "=a"(value): "Nd"(port));
    return value;
}

static inline void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %0, %1"::"a"(value), "Nd"(port));
}

static inline void io_wait() 
{
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}

static inline void outw(uint16_t port, uint16_t val) 
{
    asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

#endif