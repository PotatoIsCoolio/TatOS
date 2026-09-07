/*
################################
#           [TatOS]            #
#    File: MouseDrivers.cpp    #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "Handler.h"
#include "../HardwareManager/IO.h"

#define MOUSE_DATA_PORT   0x60
#define MOUSE_STATUS_PORT 0x64

// Ill clean up this code as I didnt make some of it. 

void TatOS::Mouse::WaitWrite()
{
    uint32_t timeout = 100000;
    while (timeout--) 
    {
        if ((inb(MOUSE_STATUS_PORT) & 2) == 0) 
            return;
    }
}

void TatOS::Mouse::WaitRead() 
{
    uint32_t timeout = 100000;
    while (timeout--) 
    {
        if ((inb(MOUSE_STATUS_PORT) & 1) == 1) 
            return;
    }
}

// Flush any leftover garbage bytes hanging in port 0x60
void TatOS::Mouse::FlushBuffer() 
{
    uint32_t timeout = 1000;
    while ((inb(MOUSE_STATUS_PORT) & 1) && timeout--) 
    {
        inb(MOUSE_DATA_PORT);
    }
}

void TatOS::Mouse::WritePort(uint8_t port, uint8_t data) 
{
    WaitWrite();
    outb(MOUSE_STATUS_PORT, port);
    WaitWrite();
    outb(MOUSE_DATA_PORT, data);
}

uint8_t TatOS::Mouse::ReadPort() 
{
    WaitRead();
    return inb(MOUSE_DATA_PORT);
}

void TatOS::Mouse::Init(TatOS::Multiboot::MultibootInfo* mb_info) 
{
    if (mb_info) 
    {
        screen_width = mb_info->framebuffer_width;
        screen_height = mb_info->framebuffer_height;
        state.x = screen_width / 2;
        state.y = screen_height / 2;
    }

    PacketCycle = 0;

    // Drain any residual boot bytes from BIOS/GRUB
    FlushBuffer();

    // Enable auxiliary mouse device
    WaitWrite();
    outb(MOUSE_STATUS_PORT, 0xA8);

    // Enable mouse interrupts & clear disable clock bit in controller command byte
    WaitWrite();
    outb(MOUSE_STATUS_PORT, 0x20);
    uint8_t status = ReadPort() | 2; // Enable AUX IRQ12
    status &= ~0x20;                 // Clear 'Disable Mouse' bit
    
    WaitWrite();
    outb(MOUSE_STATUS_PORT, 0x60);
    WaitWrite();
    outb(MOUSE_DATA_PORT, status);

    // Send "Set Defaults" command (0xF6) & consume ACK
    WritePort(0xD4, 0xF6);
    ReadPort();

    // Send "Enable Packet Streaming" command (0xF4) & consume ACK
    WritePort(0xD4, 0xF4);
    ReadPort();

    // Clear buffer one last time before starting main loop
    FlushBuffer();
}

void TatOS::Mouse::HandlePacket() 
{
    state.left_click = (packet[0] & 0x01) != 0;
    state.right_click = (packet[0] & 0x02) != 0;
    state.middle_click = (packet[0] & 0x04) != 0;

    int32_t rel_x = packet[1];
    if (packet[0] & 0x10) 
        rel_x |= 0xFFFFFF00;

    int32_t rel_y = packet[2];
    if (packet[0] & 0x20) 
        rel_y |= 0xFFFFFF00;

    state.x += rel_x;
    state.y -= rel_y;

    // Boundary Clamping
    if (state.x < 0) 
        state.x = 0;

    if (state.y < 0) 
        state.y = 0;
        
    if (state.x >= (int32_t)screen_width)  
        state.x = screen_width - 8;

    if (state.y >= (int32_t)screen_height) 
        state.y = screen_height - 8;
}

void TatOS::Mouse::HandleByte(uint8_t data)
{
    if (PacketCycle == 0 && !(data & 0x08))
    {
        return;
    }
    packet[PacketCycle++] = data;
    if (PacketCycle >= 3)
    {
        PacketCycle = 0;
        HandlePacket();
    }
}

void TatOS::Mouse::Poll() 
{
    while (inb(MOUSE_STATUS_PORT) & 0x01) 
    {
        uint8_t status = inb(MOUSE_STATUS_PORT);
        uint8_t data = inb(MOUSE_DATA_PORT);

        if (status & 0x20) 
        {
            if (PacketCycle == 0 && !(data & 0x08)) 
            {
                continue;
            }
            packet[PacketCycle++] = data;
            if (PacketCycle >= 3) 
            {
                PacketCycle = 0;
                HandlePacket();
            }
        }
    }
}