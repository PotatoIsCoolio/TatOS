/*
################################
#           [TatOS]            #
#       File: Power.cpp        #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "Power.h"
#include "../HardwareManager/IO.h"

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

void Drivers::Power::Shutdown()
{
    asm volatile("cli");

    outw(0x604, 0x2000);  
    outw(0x4004, 0x3400); 
    outw(0xB004, 0x2000); 

    uint8_t* memory = (uint8_t*)0xE0000;
    uint32_t pm1a_cnt = 0;

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
                        pm1a_cnt = fadt->PM1a_CNT_BLK;
                        break;
                    }
                }
            }
            break;
        }
    }

    if (pm1a_cnt != 0) {
        outw((uint16_t)pm1a_cnt, 0x2000 | 0x2000);
        io_wait();
        outw((uint16_t)pm1a_cnt, 0x3A00);
    }

    while (true) {
        asm volatile("hlt");
    }
}

void Drivers::Power::Reboot()
{
    asm volatile("cli");

    outb(0xCF9, 0x02);
    io_wait();
    outb(0xCF9, 0x06);
    io_wait();

    uint8_t temp = 0x02;
    for (int i = 0; i < 1000 && (temp & 0x02); i++) 
    {
        temp = inb(0x64);
    }
    outb(0x64, 0xFE);
    io_wait();

    struct {
        uint16_t limit = 0;
        uint32_t base  = 0;
    } __attribute__((packed)) null_idtr;

    asm volatile("lidt %0" : : "m"(null_idtr));
    asm volatile("int $3");

    while (true) {
        asm volatile("hlt"); // Halts the CPU
    }
}