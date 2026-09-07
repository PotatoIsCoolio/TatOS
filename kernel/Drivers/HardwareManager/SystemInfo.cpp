/*
################################
#           [TatOS]            #
#     File: SystemInfo.cpp     #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
// Potato did not make any of this. Just slightly adjusted.
#include "SystemInfo.h"
#include <cpuid.h>

static inline uint64_t rdtsc() {
    uint32_t lo, hi;
    asm volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

// Multiboot 1 Memory Map Structures passed from GRUB. Make sure this is not Multiboot 2.
struct MultibootMMapEntry {
    uint32_t size; // Size of the rest of the structure (typically 20 bytes)
    uint64_t addr;
    uint64_t len;
    uint32_t type; // 1 = Available (MULTIBOOT_MEMORY_AVAILABLE)
} __attribute__((packed));

struct MultibootInfo {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
} __attribute__((packed));

// ACPI Table Headers
struct ACPIHeader {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

struct FADT {
    ACPIHeader h;
    uint32_t firmware_ctrl;
    uint32_t dsdt;
    uint8_t  reserved;
    uint8_t  preferred_pm_profile;
    uint16_t sci_interrupt;
    uint32_t smi_command_port;
    uint8_t  acpi_enable;
    uint8_t  acpi_disable;
    uint8_t  S4BIOS_REQ;
    uint8_t  PSTATE_CNT;
    uint32_t PM1a_EVT_BLK;
    uint32_t PM1b_EVT_BLK;
    uint32_t PM1a_CNT_BLK;
    uint32_t PM1b_CNT_BLK;
} __attribute__((packed));


namespace SystemInfo 
{

    void FormatBytes(uint64_t bytes, char* out_buf) {
        // Handle 0 explicitly
        if (bytes == 0) {
            out_buf[0] = '0'; out_buf[1] = ' '; out_buf[2] = 'M'; out_buf[3] = 'B'; out_buf[4] = '\0';
            return;
        }

        uint64_t mb = bytes / (1024 * 1024);
        
        if (mb == 0) {
            uint64_t kb = bytes / 1024;
            uint32_t val = (uint32_t)kb;
            int idx = 0;
            if (val >= 1000) out_buf[idx++] = '0' + (val / 1000);
            if (val >= 100)  out_buf[idx++] = '0' + ((val / 100) % 10);
            if (val >= 10)   out_buf[idx++] = '0' + ((val / 10) % 10);
            out_buf[idx++] = '0' + (val % 10);
            out_buf[idx++] = ' '; out_buf[idx++] = 'K'; out_buf[idx++] = 'B'; out_buf[idx] = '\0';
            return;
        }

        if (mb >= 1024) {
            uint32_t whole = (uint32_t)(mb / 1024);
            uint32_t frac = (uint32_t)(((mb % 1024) * 10) / 1024);
            
            int idx = 0;
            if (whole >= 100) out_buf[idx++] = '0' + ((whole / 100) % 10);
            if (whole >= 10)  out_buf[idx++] = '0' + ((whole / 10) % 10);
            out_buf[idx++] = '0' + (whole % 10);
            
            out_buf[idx++] = '.';
            out_buf[idx++] = '0' + frac;
            out_buf[idx++] = ' '; out_buf[idx++] = 'G'; out_buf[idx++] = 'B'; 
            out_buf[idx] = '\0';
        } else {
            uint32_t val = (uint32_t)mb;
            int idx = 0;
            if (val >= 1000) out_buf[idx++] = '0' + (val / 1000);
            if (val >= 100)  out_buf[idx++] = '0' + ((val / 10) % 10);
            if (val >= 10)   out_buf[idx++] = '0' + ((val / 10) % 10);
            out_buf[idx++] = '0' + (val % 10);
            
            out_buf[idx++] = ' '; out_buf[idx++] = 'M'; out_buf[idx++] = 'B'; 
            out_buf[idx] = '\0';
        }
    }

    void GetCPUInfo(CPUInfo* info) {
        uint32_t eax, ebx, ecx, edx;

        // 1. Get CPU Vendor String ("GenuineIntel", "AuthenticAMD")
        __get_cpuid(0, &eax, &ebx, &ecx, &edx);
        *(uint32_t*)(info->vendor)     = ebx;
        *(uint32_t*)(info->vendor + 4) = edx;
        *(uint32_t*)(info->vendor + 8) = ecx;
        info->vendor[12] = '\0';

        // 2. Get Family, Model, Stepping & Feature Flags
        __get_cpuid(1, &eax, &ebx, &ecx, &edx);
        info->stepping = eax & 0xF;
        info->model    = (eax >> 4) & 0xF;
        info->family   = (eax >> 8) & 0xF;

        info->has_apic = (edx & (1 << 9)) != 0;
        info->has_sse  = (edx & (1 << 25)) != 0;
        info->has_sse2 = (edx & (1 << 26)) != 0;
        info->has_avx  = (ecx & (1 << 28)) != 0;

        // 3. Get Brand String
        info->brand_name[0] = '\0';
        uint32_t extended_max = __get_cpuid_max(0x80000000, nullptr);
        if (extended_max >= 0x80000004) {
            uint32_t* brand_ptr = (uint32_t*)info->brand_name;
            for (uint32_t i = 0; i < 3; i++) {
                __get_cpuid(0x80000002 + i, &brand_ptr[i*4], &brand_ptr[i*4 + 1], &brand_ptr[i*4 + 2], &brand_ptr[i*4 + 3]);
            }
            info->brand_name[48] = '\0';
        }
    }

    void GetMemoryStats(uint32_t multiboot_magic, uint32_t multiboot_addr, MemoryInfo* mem) {
        mem->total_ram_bytes = 0;
        mem->usable_ram_bytes = 0;
        mem->reserved_ram_bytes = 0;

        // If pointer is NULL, return early
        if (multiboot_addr == 0) return;

        MultibootInfo* mb_info = (MultibootInfo*)multiboot_addr;

        // METHOD 1: Try mem_lower and mem_upper (Bit 0) FIRST as a baseline guarantee
        if (mb_info->flags & (1 << 0)) {
            // mem_lower is in KB (typically 640 KB)
            // mem_upper is in KB (everything above 1 MB)
            uint64_t total_kb = (uint64_t)mb_info->mem_lower + (uint64_t)mb_info->mem_upper;
            mem->usable_ram_bytes = total_kb * 1024;
            mem->total_ram_bytes = mem->usable_ram_bytes;
        }

        // METHOD 2: Try parsing full mmap (Bit 6) if present and nonzero
        if ((mb_info->flags & (1 << 6)) && mb_info->mmap_addr != 0 && mb_info->mmap_length > 0) {
            uint8_t* mmap_ptr = (uint8_t*)mb_info->mmap_addr;
            uint8_t* mmap_end = mmap_ptr + mb_info->mmap_length;

            uint64_t mmap_usable = 0;
            uint64_t mmap_reserved = 0;

            while (mmap_ptr < mmap_end) {
                MultibootMMapEntry* entry = (MultibootMMapEntry*)mmap_ptr;

                if (entry->type == 1) { // Available RAM
                    mmap_usable += entry->len;
                } else {
                    mmap_reserved += entry->len;
                }

                uint32_t advance = entry->size + sizeof(uint32_t);
                if (advance == 0) break; // Prevent infinite loop on bad structure
                mmap_ptr += advance;
            }

            // Only overwrite if mmap parsing actually found memory
            if (mmap_usable > 0) {
                mem->usable_ram_bytes = mmap_usable;
                mem->reserved_ram_bytes = mmap_reserved;
                mem->total_ram_bytes = mmap_usable + mmap_reserved;
            }
        }
    }

    uint16_t GetACPIPort() {
        uint8_t* memory = (uint8_t*)0xE0000;
        for (uint32_t i = 0; i < 0x20000; i += 16) {
            if (memory[i] == 'R' && memory[i+1] == 'S' && memory[i+2] == 'D' && memory[i+3] == ' ' &&
                memory[i+4] == 'P' && memory[i+5] == 'T' && memory[i+6] == 'R' && memory[i+7] == ' ') {
                
                uint32_t rsdt_addr = *(uint32_t*)&memory[i + 16];
                ACPIHeader* rsdt = (ACPIHeader*)rsdt_addr;

                if (rsdt) {
                    uint32_t entries = (rsdt->length - sizeof(ACPIHeader)) / 4;
                    uint32_t* table_ptrs = (uint32_t*)(rsdt_addr + sizeof(ACPIHeader));

                    for (uint32_t t = 0; t < entries; t++) {
                        ACPIHeader* header = (ACPIHeader*)table_ptrs[t];
                        if (header && header->signature[0] == 'F' && header->signature[1] == 'A' &&
                            header->signature[2] == 'C' && header->signature[3] == 'P') {
                            FADT* fadt = (FADT*)header;
                            return (uint16_t)fadt->PM1a_CNT_BLK;
                        }
                    }
                }
                break;
            }
        }
        return 0;
    }

    uint64_t GetTSCCycles() {
        return rdtsc();
    }

    HardwareStats FetchAll(uint32_t multiboot_magic, uint32_t multiboot_addr) {
        HardwareStats stats;
        GetCPUInfo(&stats.cpu);
        GetMemoryStats(multiboot_magic, multiboot_addr, &stats.memory);
        stats.acpi_pm1a_port = GetACPIPort();
        stats.tsc_cycles = GetTSCCycles();
        return stats;
    }
}