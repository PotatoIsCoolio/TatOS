/*
################################
#           [TatOS]            #
#       File: Images.cpp       #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "Handler.h"
#include "../../Filesystem/TatoFS.h"
#include "ImageHandler/stb_image.h"
#include <cstring>

extern "C" {
    void* kmalloc(size_t size);
    void kfree(void* ptr);
}

// 1. Load and decode image once into memory
LoadedImage TatOS::Screen::LoadImageFromFile(const char* Filename) 
{
    LoadedImage Image;
    File file;

    if (!global_fs.ReadFile(Filename, &file) || file.data == nullptr || file.size == 0) {
        return Image; 
    }

    int ImageWidth = 0, ImageHeight = 0, Channels = 0;
    uint8_t* RawPixels = stbi_load_from_memory(reinterpret_cast<const uint8_t*>(file.data), static_cast<int>(file.size), &ImageWidth,  &ImageHeight, &Channels, 4);

    if (!RawPixels) 
        return Image;

    Image.width = static_cast<uint32_t>(ImageWidth);
    Image.height = static_cast<uint32_t>(ImageHeight);
    
    // Allocate buffer for ARGB pixels using my kmalloc
    Image.pixels = static_cast<uint32_t*>(kmalloc(Image.width * Image.height * sizeof(uint32_t)));

    if (Image.pixels) 
    {
        for (uint32_t i = 0; i < Image.width * Image.height; i++) 
        {
            uint8_t r = RawPixels[i * 4 + 0];
            uint8_t g = RawPixels[i * 4 + 1];
            uint8_t b = RawPixels[i * 4 + 2];
            uint8_t a = RawPixels[i * 4 + 3];

            // Store in 0xAARRGGBB format
            Image.pixels[i] = (a << 24) | (r << 16) | (g << 8) | b;
        }
    }

    stbi_image_free(RawPixels);
    return Image;
}

// Draw function using cached raw pixels.
// I did not fully make this function as some how the images were duping lol.
void TatOS::Screen::DrawImage(const LoadedImage& Image, uint32_t x, uint32_t y, uint32_t ImageWidth, uint32_t ImageHeight) 
{
    if (!Image.pixels || !BackBuffer || Image.width == 0 || Image.height == 0) 
        return;

    uint32_t RenderWidth = (ImageWidth > 0) ? ImageWidth : Image.width;
    uint32_t RenderHeight = (ImageHeight > 0) ? ImageHeight : Image.height;
    // Good Practice Mr P!!
    if (x >= Width || y >= Height) 
        return;

    uint32_t DrawWidth = RenderWidth;
    uint32_t DrawHeight = RenderHeight;

    if (x + DrawWidth > Width) 
        DrawWidth = Width - x;

    if (y + DrawHeight > Height) 
        DrawHeight = Height - y;

    if (RenderWidth == Image.width && RenderHeight == Image.height) 
    {
        for (uint32_t py = 0; py < DrawHeight; py++) 
        {
            uint32_t* dst = &BackBuffer[(y + py) * Width + x];
            
            uint32_t src_y = Image.height - 1 - py;
            const uint32_t* src_row = &Image.pixels[src_y * Image.width];
            for (uint32_t px = 0; px < DrawWidth; px++) 
            {
                dst[px] = src_row[Image.width - 1 - px];
            }
        }
        return;
    }

    for (uint32_t py = 0; py < DrawHeight; py++) 
    {
        uint32_t* dst = &BackBuffer[(y + py) * Width + x];
        uint32_t src_y = Image.height - 1 - ((py * Image.height) / RenderHeight);
        const uint32_t* src_row = &Image.pixels[src_y * Image.width];
        for (uint32_t px = 0; px < DrawWidth; px++) 
        {
            uint32_t src_x = Image.width - 1 - ((px * Image.width) / RenderWidth);
            dst[px] = src_row[src_x];
        }
    }
}