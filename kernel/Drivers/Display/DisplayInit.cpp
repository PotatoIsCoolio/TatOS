/*
################################
#           [TatOS]            #
#    File: DisplayInit.cpp     #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "Handler.h"

static uint32_t BackBufferStorage[1920 * 1080];

// Not made by Tato.
static void fast_memcpy(void* dest, const void* src, uint32_t count) {
    uint64_t* d64 = (uint64_t*)dest;
    const uint64_t* s64 = (const uint64_t*)src;
    uint32_t count64 = count / 8;

    for (uint32_t i = 0; i < count64; i++) {
        d64[i] = s64[i];
    }
}

void TatOS::Screen::Init(TatOS::Multiboot::MultibootInfo* MultibootInfo)
{
    Framebuffer = (uint32_t*)(uintptr_t)MultibootInfo->framebuffer_addr;
    Pitch = MultibootInfo->framebuffer_pitch / 4;
    Width = MultibootInfo->framebuffer_width;
    Height = MultibootInfo->framebuffer_height;

    BackBuffer = BackBufferStorage;
}

void TatOS::Screen::PlacePixel(uint32_t x, uint32_t y, uint32_t Color)
{
    if (x < TatOS::Screen::Width && y < TatOS::Screen::Height)
    {
        TatOS::Screen::BackBuffer[y * TatOS::Screen::Width + x] = Color;
    }
}

void TatOS::Screen::Clear(uint32_t Color)
{
    uint32_t TotalPixels = TatOS::Screen::Width * TatOS::Screen::Height;
    for (uint32_t Iteration = 0; Iteration < TotalPixels; Iteration++)
    {
        TatOS::Screen::BackBuffer[Iteration] = Color;
    }
}

void TatOS::Screen::SwapBuffers() 
{
    // If Pitch == Width then we can copy the entire screen with 1 single memory block
    if (TatOS::Screen::Pitch == TatOS::Screen::Width) 
    {
        fast_memcpy(Framebuffer, BackBuffer, TatOS::Screen::Width * TatOS::Screen::Height * sizeof(uint32_t));
    } 
    else 
    {
        // Copy line by line if hardware padding exists
        for (uint32_t y = 0; y < TatOS::Screen::Height; y++) 
        {
            fast_memcpy(&Framebuffer[y * TatOS::Screen::Pitch], &BackBuffer[y * TatOS::Screen::Width], TatOS::Screen::Width * sizeof(uint32_t));
        }
    }
}

// Not made by Tato.
void TatOS::Screen::SwapMouseRegion(int32_t x, int32_t y) 
{
    int32_t startX = (x - 16 < 0) ? 0 : x - 16;
    int32_t startY = (y - 16 < 0) ? 0 : y - 16;

    int32_t endX = (x + 32 >= (int32_t)TatOS::Screen::Width) ? TatOS::Screen::Width - 1 : x + 32;
    int32_t endY = (y + 32 >= (int32_t)Height) ? Height - 1 : y + 32;

    for (int32_t cy = startY; cy < endY; cy++) {
        for (int32_t cx = startX; cx < endX; cx++) {
            Framebuffer[cy * Pitch + cx] = BackBuffer[cy * TatOS::Screen::Width + cx];
        }
    }
}