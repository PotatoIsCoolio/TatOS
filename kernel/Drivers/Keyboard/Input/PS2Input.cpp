/*
################################
#           [TatOS]            #
#      File: PS2Input.cpp      #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "PS2Input.h"
#include "../../HardwareManager/IO.h"
#define PS2_DATA_PORT   0x60
#define PS2_STATUS_PORT 0x64

bool TatOS::PS2Input::Poll(TatOS::Keyboard& keyboard, TatOS::Mouse& mouse, char& key)
{
    key = 0;

    uint8_t status = inb(PS2_STATUS_PORT); // Port = 0x64 :p

    if (!(status & 0x01))
        return false;

    uint8_t data = inb(PS2_DATA_PORT);

    if (status & 0x20)
        mouse.HandleByte(data);
    else
        key = keyboard.HandleScancode(data);

    return true;
}
