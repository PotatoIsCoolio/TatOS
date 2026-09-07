/*
################################
#           [TatOS]            #
#        File: Power.h         #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#pragma once
#include <stdint.h>

namespace Drivers::Power
{
    void Shutdown();
    void Reboot();
}