/*
################################
#           [TatOS]            #
#  File: KeyboardDrivers.cpp   #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
// These are my old Keyboard drives that work pretty well.

#include "Handler.h"
#include "../HardwareManager/IO.h"

#define KEYBOARD_DATA_PORT   0x60
#define KEYBOARD_STATUS_PORT 0x64

// Special Chars on keyboard.
#define KEY_BACKSPACE 0x0E
#define KEY_ENTER     0x1C
#define KEY_TAB       0x0F
#define KEY_ESC       0x01
#define KEY_UP        0x48
#define KEY_DOWN      0x50
#define KEY_LEFT      0x4B
#define KEY_RIGHT     0x4D

static const char KeyboardTable[128] = 
{
    0, 0, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0,' ', 0,
};

static const char KeyboardTable_shifted[128] = 
{
    0, 0, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,'A','S','D','F','G','H','J','K','L',':','"','~',
    0,'|','Z','X','C','V','B','N','M','<','>','?', 0,
    '*', 0,' ', 0,
};

char TatOS::Keyboard::ScancodeToAscii(uint8_t ScanCode) 
{
    if (ScanCode & 0x80) 
        return 0;

    return IsShiftPressed ? KeyboardTable_shifted[ScanCode] : KeyboardTable[ScanCode];
}

char TatOS::Keyboard::HandleScancode(uint8_t ScanCode) 
{
    // Shift pressed
    if (ScanCode == 0x2A || ScanCode == 0x36) 
    {
        IsShiftPressed = 1;
        return 0;
    }

    // Shift released
    if (ScanCode == 0xAA || ScanCode == 0xB6) 
    {
        IsShiftPressed = 0;
        return 0;
    }

    // Ignore key releases
    if (ScanCode & 0x80) 
    {
        return 0;
    }

    return ScancodeToAscii(ScanCode);
}

void TatOS::Keyboard::InitKeyboard() 
{
    while (inb(KEYBOARD_STATUS_PORT) & 1) 
    {
        inb(KEYBOARD_DATA_PORT);
    }
    IsShiftPressed = 0;
}
