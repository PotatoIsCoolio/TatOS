/*
################################
#           [TatOS]            #
#       File: PS2Input.h       #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#ifndef PS2Input_H
#define PS2Input_H

#include "../../Mouse/Handler.h"
#include "../../Keyboard/Handler.h"

class TatOS::PS2Input
{
    public:
        bool Poll(TatOS::Keyboard& keyboard, TatOS::Mouse& mouse, char& key);
};
#endif