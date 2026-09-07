/*
################################
#           [TatOS]            #
#       File: Handler.h        #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#ifndef KeyboardHandler_H
#define KeyboardHandler_H

#include <stdint.h>

namespace TatOS
{
    class Keyboard
    {
        private:
            uint8_t IsShiftPressed;
            char ScancodeToAscii(uint8_t scancode);

        public:
            void InitKeyboard();
            char HandleScancode(uint8_t scancode);
        };
}

#endif