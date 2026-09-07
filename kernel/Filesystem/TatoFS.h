/*
################################
#           [TatOS]            #
#        File: TatoFS.h        #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#pragma once
#include "File.h"
#include "../Console/Console.h"

class TatoFS 
{
    private:
        uintptr_t RamDiskStart;
        uintptr_t RamDiskEnd;

        static size_t octal_to_bin(const char* String, size_t size);
        static bool StringEquals(const char* String1, const char* String2);

    public:
        void init(uintptr_t StartAddress, uintptr_t EndAddress);
        bool ReadFile(const char* FileName, File* Outfile);
        void ListFiles(TatOSConsole* console);
};

extern TatoFS global_fs;