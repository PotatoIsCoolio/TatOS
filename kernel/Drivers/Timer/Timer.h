/*
################################
#           [TatOS]            #
#        File: Timer.h         #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

namespace Drivers 
{
    class PIT // Pit also known as Programmable Interval Timers
    {
        public:
            static void Initialize(uint32_t frequency);
            static void OnTick();
            static void Sleep(uint32_t milliseconds);
            static void SleepMicroseconds(uint32_t microseconds);
            static uint64_t GetTicks();
            static uint64_t GetTicksMicro();

        private:
            static volatile uint64_t ticks;
            static uint32_t frequency_hz;
    };
}

#endif