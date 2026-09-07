/*
################################
#           [TatOS]            #
#       File: Handler.h        #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#ifndef MouseHander_H
#define MouseHander_H
#include <stdint.h>
#include "Kernel.h"

namespace TatOS
{
    struct MouseState 
    {
        int32_t x = 200;
        int32_t y = 200;
        bool left_click = false;
        bool right_click = false;
        bool middle_click = false;
    };

    class Mouse 
    {
        private:
            uint8_t packet[3];
            uint8_t PacketCycle = 0;
            uint32_t screen_width = 1920;
            uint32_t screen_height = 1080;
                
            void WaitWrite();
            void WaitRead();
            void FlushBuffer();
            void WritePort(uint8_t port, uint8_t data);
            uint8_t ReadPort();
            void HandlePacket();

        public:
            MouseState state;
            void Init(TatOS::Multiboot::MultibootInfo* MultibootInfo);
            void HandleByte(uint8_t data);
            void Poll();
    };
}

#endif