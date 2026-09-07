/*
################################
#           [TatOS]            #
#      File: hwstats.cpp       #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "../../Console.h"
#include "../../../Colors.h"
#include "../../../Drivers/HardwareManager/SystemInfo.h"
#include "../../../Kernel.h"

extern MultibootInfo* MultibootInfoStat;

void CmdHWStats(TatOSConsole* ConsoleThingy, const char* Arguments)
{
    char ram_str[32];
    uint32_t MultiBootAddress = reinterpret_cast<uint32_t>(MultibootInfoStat);
    
    HardwareStats stats = SystemInfo::FetchAll(0x2BADB002, MultiBootAddress);
    SystemInfo::FormatBytes(stats.memory.usable_ram_bytes, ram_str);

    ConsoleThingy->AddLine("CPU Vendor:", TatOSColors::Core::White);
    ConsoleThingy->AddLine(stats.cpu.vendor, TatOSColors::Pinks::HotPink);

    if (stats.cpu.brand_name[0] != '\0') 
    {
        ConsoleThingy->AddLine("CPU Model:", TatOSColors::Core::White);
        ConsoleThingy->AddLine(stats.cpu.brand_name, TatOSColors::Pinks::HotPink);
    }
    
    ConsoleThingy->AddLine("Usable RAM:", TatOSColors::Core::White);
    ConsoleThingy->AddLine(ram_str, TatOSColors::Pinks::HotPink);
}