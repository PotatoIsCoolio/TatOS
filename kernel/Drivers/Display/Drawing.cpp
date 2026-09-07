/*
################################
#           [TatOS]            #
#      File: Drawing.cpp       #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "Handler.h"
#include <cstring>

void TatOS::Screen::DrawChar(char c, uint32_t x, uint32_t y, uint32_t ForegroundColor, uint32_t BackgroundColor)
{
    uint8_t ascii = (uint8_t)c;

    if (ascii > 127) 
        ascii = '?'; // This is for like special chars and like emojis and stuff.

    uint8_t CharacterWidth = font_widths[ascii];
    for (uint32_t row = 0; row < FontHeight; row++)
    {
        uint16_t RowBits = font_bitmap[ascii][row];
        for (uint32_t col = 0; col < font_bitmap_width; col++)
        {
            if (RowBits & (1 << (15 - col)))
            {
                PlacePixel(x + col, y + row, ForegroundColor);
            }
            else if (BackgroundColor != 0x00000000 && col < CharacterWidth)
            {
                PlacePixel(x + col, y + row, BackgroundColor);
            }
        }
    }
}

void TatOS::Screen::DrawString(const char* str, uint32_t x, uint32_t y, uint32_t ForegroundColor, uint32_t BackgroundColor)
{
    uint32_t CursorXCord = x;
    uint32_t CursorYCord = y;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
        {
            CursorXCord = x;
            CursorYCord += FontHeight + 2; 
            continue;
        }

        uint8_t ascii = (uint8_t)str[i];
        if (ascii > 127) 
            ascii = '?';

        DrawChar(str[i], CursorXCord, CursorYCord, ForegroundColor, BackgroundColor);
        CursorXCord += font_widths[ascii] + 1;
    }
}

void TatOS::Screen::DrawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
    // Top and bottom edges
    for (uint32_t iteration = x; iteration < x + w; iteration++)
    {
        PlacePixel(iteration, y, color);
        PlacePixel(iteration, y + h - 1, color);
    }
    // Left and right edges
    for (uint32_t iteration = y; iteration < y + h; iteration++)
    {
        PlacePixel(x, iteration, color);
        PlacePixel(x + w - 1, iteration, color);
    }
}

void TatOS::Screen::FillRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
    for (uint32_t Rows = y; Rows < y + h; Rows++)
    {
        for (uint32_t col = x; col < x + w; col++)
        {
            PlacePixel(col, Rows, color);
        }
    }
}

// Credits to my homeboy Jack Elton Bresenham.
void TatOS::Screen::DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color)
{
    int32_t dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int32_t dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
    int32_t sx = (x0 < x1) ? 1 : -1;
    int32_t sy = (y0 < y1) ? 1 : -1;
    int32_t err = dx - dy;

    while (true)
    {
        PlacePixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;

        int32_t e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void TatOS::Screen::DrawCircle(int32_t center_x, int32_t center_y, int32_t radius, uint32_t color)
{
    int32_t x = radius;
    int32_t y = 0;
    int32_t err = 0;

    while (x >= y)
    {
        PlacePixel(center_x + x, center_y + y, color);
        PlacePixel(center_x + y, center_y + x, color);
        PlacePixel(center_x - y, center_y + x, color);
        PlacePixel(center_x - x, center_y + y, color);
        PlacePixel(center_x - x, center_y - y, color);
        PlacePixel(center_x - y, center_y - x, color);
        PlacePixel(center_x + y, center_y - x, color);
        PlacePixel(center_x + x, center_y - y, color);

        if (err <= 0)
        {
            y += 1;
            err += 2 * y + 1;
        }
        if (err > 0)
        {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

void TatOS::Screen::DrawCursor(int32_t x, int32_t y, bool MouseClicked) {
    uint32_t fill_color = MouseClicked ? 0x00FF0000 : 0x00FFFFFF; // Red if clicked White if default

    for (int CursorY = 0; CursorY < 16; CursorY++) 
    {
        for (int CursorX = 0; CursorX < 12; CursorX++) 
        {
            uint8_t pixel = MouseCursorSprite[CursorY][CursorX];

            if (pixel == 1) 
            {
                PlacePixel(x + CursorX, y + CursorY, 0x00000000); // Black outline
            } 
            else if (pixel == 2) 
            {
                PlacePixel(x + CursorX, y + CursorY, fill_color); // Inside fill
            }
        }
    }
}

void TatOS::Screen::EraseCursor(int32_t x, int32_t y, uint32_t BackgroundColor) 
{
    for (int CursorY = 0; CursorY < 16; CursorY++) 
    {
        for (int CursorX = 0; CursorX < 12; CursorX++) 
        {
            if (MouseCursorSprite[CursorY][CursorX] != 0) 
            {
                PlacePixel(x + CursorX, y + CursorY, BackgroundColor);
            }
        }
    }
}