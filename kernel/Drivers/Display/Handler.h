/*
################################
#           [TatOS]            #
#       File: Handler.h        #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#ifndef DisplayDrivers_H
#define DisplayDrivers_H
#include <stdint.h>
#include "../../Kernel.h"

namespace TatOS
{
    class Screen 
        {
            private:
                uint32_t* BackBuffer; // Secondary RAM buffer
                uint32_t* Framebuffer;  // Actual VESA Video RAM pointer
                uint32_t screen_size_pixels; // Total width * height
                uint32_t Pitch;
                uint32_t Width;
                uint32_t Height;

            public:

                void Init(TatOS::Multiboot::MultibootInfo * MultibootInfo);
                void PlacePixel(uint32_t x, uint32_t y, uint32_t color);
                void Clear(uint32_t color = 0x00);

                void SwapBuffers();
                void SwapMouseRegion(int32_t x, int32_t y);

                void DrawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
                void FillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
                void DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);
                void DrawCircle(int32_t center_x, int32_t center_y, int32_t radius, uint32_t color);

                void DrawChar(char c, uint32_t x, uint32_t y, uint32_t fg_color, uint32_t BackgroundColor);
                void DrawString(const char* str, uint32_t x, uint32_t y, uint32_t fg_color, uint32_t BackgroundColor);

                void DrawCursor(int32_t x, int32_t y, bool Clicked);
                void EraseCursor(int32_t x, int32_t y, uint32_t BackgroundColor);

                LoadedImage LoadImageFromFile(const char* Filename);
                void DrawImage(const LoadedImage& Image, uint32_t x, uint32_t y, uint32_t ImageWidth = 0, uint32_t ImageHeight = 0);
        };
};
#endif