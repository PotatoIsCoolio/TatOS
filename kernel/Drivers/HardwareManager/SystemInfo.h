/*
################################
#           [TatOS]            #
#      File: SystemInfo.h      #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#pragma once
#include <stdint.h>
#include <stddef.h>

struct MultibootInfo; 
extern MultibootInfo* MultibootInfoStat;
extern uint32_t MultibootMagicStat;

struct CPUInfo 
{
    char vendor[13];
    char brand_name[49];
    uint32_t family;
    uint32_t model;
    uint32_t stepping;
    bool has_sse;
    bool has_sse2;
    bool has_avx;
    bool has_apic;
};

struct MemoryInfo
{
    uint64_t total_ram_bytes;
    uint64_t usable_ram_bytes;
    uint64_t reserved_ram_bytes;
};

struct HardwareStats
{
    CPUInfo cpu;
    MemoryInfo memory;
    uint16_t acpi_pm1a_port;
    uint64_t tsc_cycles;
};

namespace SystemInfo
{
    void FormatBytes(uint64_t bytes, char* out_buf);
    void GetCPUInfo(CPUInfo* info);
    void GetMemoryStats(uint32_t MultiBootMagic, uint32_t MultiBootAddress, MemoryInfo* mem);
    uint16_t GetACPIPort();
    uint64_t GetTSCCycles();
    
    // Main summary collector
    HardwareStats FetchAll(uint32_t MultiBootMagic, uint32_t MultiBootAddress);
}