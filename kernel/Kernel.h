/*
################################
#           [TatOS]            #
#        File: Kernel.h        #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#pragma once
#include <stdint.h>
#include "Fonts/ArialFont.h"

/* Multiboot Flags */
#define MULTIBOOT_ALIGN       (1 << 0)
#define MULTIBOOT_MEMINFO     (1 << 1)
#define MULTIBOOT_VIDMODE     (1 << 2) // Request video mode from GRUB

#define MULTIBOOT_MAGIC       0x1BADB002
#define MULTIBOOT_FLAGS       0x00000007 // ALIGN | MEMINFO | VIDMODE
#define MULTIBOOT_CHECKSUM    (uint32_t)(-(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS))


// 0 = Transparent
// 1 = Black Outline
// 2 = White Fill
static const uint8_t MouseCursorSprite[16][12] = 
{
    {1,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0,0,0,0,0},
    {1,2,1,0,0,0,0,0,0,0,0,0},
    {1,2,2,1,0,0,0,0,0,0,0,0},
    {1,2,2,2,1,0,0,0,0,0,0,0},
    {1,2,2,2,2,1,0,0,0,0,0,0},
    {1,2,2,2,2,2,1,0,0,0,0,0},
    {1,2,2,2,2,2,2,1,0,0,0,0},
    {1,2,2,2,2,2,2,2,1,0,0,0},
    {1,2,2,2,2,2,1,1,1,1,0,0},
    {1,2,2,1,2,2,1,0,0,0,0,0},
    {1,2,1,0,1,2,2,1,0,0,0,0},
    {1,1,0,0,1,2,2,1,0,0,0,0},
    {0,0,0,0,0,1,2,2,1,0,0,0},
    {0,0,0,0,0,1,2,2,1,0,0,0},
    {0,0,0,0,0,0,1,1,0,0,0,0}
};

struct MultibootInfo 
{
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
    
    uint64_t framebuffer_addr;  // Pointer to pixel memory
    uint32_t framebuffer_pitch; // Bytes per scanline (width * 4 + padding)
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
    uint8_t  framebuffer_type;
} __attribute__((packed));

struct LoadedImage {
    uint32_t* pixels = nullptr;
    uint32_t width = 0;
    uint32_t height = 0;
};

namespace TatOS
{
    class PS2Input;
    
    namespace Multiboot
    {
        constexpr uint32_t Multiboot_Align   (1 << 0);
        constexpr uint32_t Multiboot_Meminfo (1 << 1);
        constexpr uint32_t Multiboot_Vidmode (1 << 2) ;// Request video mode from GRUB

        constexpr uint32_t Multiboot_magic = 0x1BADB002;
        constexpr uint32_t Multiboot_flags = 0x00000007; // ALIGN | MEMINFO | VIDMODE
        constexpr uint32_t Multiboot_Checksum = (uint32_t)(-(Multiboot_magic + Multiboot_flags));

        struct MultibootInfo 
        {
            uint32_t flags;
            uint32_t mem_lower;
            uint32_t mem_upper;
            uint32_t boot_device;
            uint32_t cmdline;
            uint32_t mods_count;
            uint32_t mods_addr;
            uint32_t num;
            uint32_t size;
            uint32_t addr;
            uint32_t shndx;
            uint32_t mmap_length;
            uint32_t mmap_addr;
            uint32_t drives_length;
            uint32_t drives_addr;
            uint32_t config_table;
            uint32_t boot_loader_name;
            uint32_t apm_table;
            uint32_t vbe_control_info;
            uint32_t vbe_mode_info;
            uint16_t vbe_mode;
            uint16_t vbe_interface_seg;
            uint16_t vbe_interface_off;
            uint16_t vbe_interface_len;
            
            uint64_t framebuffer_addr;  // Pointer to pixel memory
            uint32_t framebuffer_pitch; // Bytes per scanline (width * 4 + padding)
            uint32_t framebuffer_width;
            uint32_t framebuffer_height;
            uint8_t  framebuffer_bpp;
            uint8_t  framebuffer_type;
        } 
        __attribute__((packed));
    }

};