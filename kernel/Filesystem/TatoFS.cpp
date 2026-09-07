/*
################################
#           [TatOS]            #
#       File: TatoFS.cpp       #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "TatoFS.h"
#include "../Colors.h"

TatoFS global_fs;
size_t TatoFS::octal_to_bin(const char* String, size_t size) 
{
    size_t n = 0;
    for (size_t i = 0; i < size && String[i] >= '0' && String[i] <= '7'; ++i) 
    {
        n = (n << 3) + (String[i] - '0');
    }
    return n;
}

// Instead of using #include <cstring> we will just make our own just incase I need to adjust.
bool TatoFS::StringEquals(const char* String1, const char* String2) 
{
    while (*String1 && (*String1 == *String2)) 
    {
        String1++; 
        String2++; 
    }
    return *String1 == *String2;
}

void TatoFS::init(uintptr_t StartAddress, uintptr_t EndAddress) 
{
    RamDiskStart = StartAddress;
    RamDiskEnd = EndAddress;
    // Ill hopefully add more on to this later. Else ill just remove this
}

bool TatoFS::ReadFile(const char* FileName, File* Outfile) 
{
    uintptr_t ptr = RamDiskStart;

    while (ptr < RamDiskEnd) // GOOD PRACTICE MR P!!!
    {
        TarHeader* header = reinterpret_cast<TarHeader*>(ptr);

        if (header->name[0] == '\0') 
            break;

        size_t FileSize = octal_to_bin(header->size, 11);

        // Strip leading "./" if present in tar path so its easier.
        const char* Entryname = header->name;
        if (Entryname[0] == '.' && Entryname[1] == '/') 
        {
            Entryname += 2;
        }

        if (StringEquals(Entryname, FileName)) 
        {
            Outfile->name = Entryname;
            Outfile->data = reinterpret_cast<const uint8_t*>(ptr + 512);
            Outfile->size = FileSize;
            Outfile->is_directory = (header->typeflag == '5');
            return true;
        }

        ptr += 512 + ((FileSize + 511) & ~511);
    }

    return false;
}

void TatoFS::ListFiles(TatOSConsole* console) 
{
    uintptr_t ptr = RamDiskStart;

    while (ptr < RamDiskEnd) 
    {
        TarHeader* header = reinterpret_cast<TarHeader*>(ptr);

        if (header->name[0] == '\0') 
            break;

        size_t FileSize = octal_to_bin(header->size, 11);

        const char* Entryname = header->name;
        if (Entryname[0] == '.' && Entryname[1] == '/') 
        {
            Entryname += 2;
        }

        if (Entryname[0] != '\0') 
        {
            console->AddLine(Entryname, TatOSColors::Pinks::Pink);
        }

        ptr += 512 + ((FileSize + 511) & ~511);
    }
}