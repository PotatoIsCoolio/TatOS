/*
################################
#           [TatOS]            #
#       File: Timer.cpp        #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "Timer.h"
#include "../HardwareManager/IO.h"
// https://wiki.osdev.org/Programmable_Interval_Timer
#define PIT_CHANNEL_0    0x40
#define PIT_COMMAND_PORT 0x43
#define PIT_BASE_FREQ    1193182U // Hardware crystal oscillator frequency in Hz

namespace Drivers 
{
    volatile uint64_t PIT::ticks = 0;
    uint32_t PIT::frequency_hz = 1000;

    void PIT::Initialize(uint32_t frequency) 
    {
        frequency_hz = frequency;

        // Calculate reload divisor (1,193,182 Hz / target frequency)
        // e.g. 1193182 / 1000 = 1193 divisor (~1000 Hz / 1ms per interrupt)
        uint32_t divisor = PIT_BASE_FREQ / frequency;

        outb(PIT_COMMAND_PORT, 0x36);
        outb(PIT_CHANNEL_0, (uint8_t)(divisor & 0xFF)); 
        outb(PIT_CHANNEL_0, (uint8_t)((divisor >> 8) & 0xFF)); 
    }

    void PIT::OnTick() 
    {
        ticks = ticks + 1;
    }

    uint64_t PIT::GetTicks() 
    {
        return ticks;
    }

    uint64_t PIT::GetTicksMicro() 
    {
        // 1 tick is 1000 Hz = 1 ms = 1000 microseconds
        return ticks * 1000ULL;
    }

    void PIT::Sleep(uint32_t milliseconds) 
    {
        uint64_t start_ticks = ticks;
        uint64_t ticks_to_wait = (milliseconds * frequency_hz) / 1000;

        while ((ticks - start_ticks) < ticks_to_wait) {
            // hlt will put the CPU into a low power state until IRQ0.
            asm volatile("hlt");
        }
    }

    void PIT::SleepMicroseconds(uint32_t microseconds) 
    {
        uint32_t ms = microseconds / 1000;

        if (ms == 0) 
            ms = 1;

        Sleep(ms);
    }
}
