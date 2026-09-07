/*
################################
#           [TatOS]            #
#        File: Colors.h        #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#pragma once
#include <stdint.h>

namespace TatOSColors
{
    // Ai Generated Colors.
    namespace Core
    {
        constexpr uint32_t Transparent = 0x00000000;
        constexpr uint32_t Black       = 0x00000000;
        constexpr uint32_t White       = 0x00FFFFFF;
        constexpr uint32_t OffWhite    = 0x00F0F0F0;
    }

    namespace Grayscale
    {
        constexpr uint32_t DarkGrey  = 0x00222222;
        constexpr uint32_t Grey      = 0x00555555;
        constexpr uint32_t MidGrey   = 0x00888888;
        constexpr uint32_t LightGrey = 0x00CCCCCC;
    }

    namespace Reds
    {
        constexpr uint32_t Red     = 0x00FF0000;
        constexpr uint32_t Crimson = 0x00DC143C;
        constexpr uint32_t DarkRed = 0x008B0000;
    }

    namespace Greens
    {
        constexpr uint32_t Green     = 0x0000FF00;
        constexpr uint32_t DarkGreen = 0x00008000;
        constexpr uint32_t Lime      = 0x0032CD32;
        constexpr uint32_t Mint      = 0x0098FF98;
    }

    namespace Blues
    {
        constexpr uint32_t Blue      = 0x000000FF;
        constexpr uint32_t LightBlue = 0x00ADD8E6;
        constexpr uint32_t DeepBlue  = 0x0000008B;
        constexpr uint32_t RoyalBlue = 0x004169E1;
    }

    namespace Yellows
    {
        constexpr uint32_t Yellow = 0x00FFFF00;
        constexpr uint32_t Gold   = 0x00FFD700;
        constexpr uint32_t Amber  = 0x00FFBF00;
    }

    namespace Oranges
    {
        constexpr uint32_t Orange     = 0x00FFA500;
        constexpr uint32_t DarkOrange = 0x00FF8C00;
    }

    namespace Cyans
    {
        constexpr uint32_t Cyan = 0x0000FFFF;
        constexpr uint32_t Aqua = 0x0000FFFF;
        constexpr uint32_t Teal = 0x00008080;
    }

    namespace Purples
    {
        constexpr uint32_t Purple  = 0x00800080;
        constexpr uint32_t Magenta = 0x00FF00FF;
        constexpr uint32_t Violet  = 0x00EE82EE;
        constexpr uint32_t Indigo  = 0x004B0082;
    }

    namespace Pinks
    {
        constexpr uint32_t Pink    = 0x00FFC0CB;
        constexpr uint32_t HotPink = 0x00FF69B4;
    }

    namespace Browns
    {
        constexpr uint32_t Brown       = 0x00A52A2A;
        constexpr uint32_t SaddleBrown = 0x008B4513;
    }

    namespace Terminal
    {
        constexpr uint32_t TermGreen = 0x0000FF00;
        constexpr uint32_t TermAmber = 0x00FFB000;
        constexpr uint32_t TermCyan  = 0x0000E5FF;
        constexpr uint32_t TermDim   = 0x00444444;
    }

    namespace UI
    {
        constexpr uint32_t DesktopBg    = 0x001A1A2E;
        constexpr uint32_t ConsoleBg    = 0x00000000;
        constexpr uint32_t WindowHeader = 0x000F3460;
        constexpr uint32_t WindowBg     = 0x0016213E;
        constexpr uint32_t TaskbarBg    = 0x000F0F1A;
    }

    namespace Status
    {
        constexpr uint32_t Success = 0x002ECC71;
        constexpr uint32_t Warning = 0x00F1C40F;
        constexpr uint32_t Error   = 0x00E74C3C;
        constexpr uint32_t Info    = 0x003498DB;
    }
}