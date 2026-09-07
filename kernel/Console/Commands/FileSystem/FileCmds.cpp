/*
################################
#           [TatOS]            #
#      File: FileCmds.cpp      #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "../../Console.h"
#include "../../../Filesystem/TatoFS.h"
#include "../../../Colors.h"

void CmdLs(TatOSConsole* ConsoleThingy, const char*) 
{
    global_fs.ListFiles(ConsoleThingy);
}

void CmdCat(TatOSConsole* ConsoleThingy, const char* Arguments) 
{
    // Trim leading spaces
    while (*Arguments == ' ') Arguments++;

    if (*Arguments == '\0') 
    {
        ConsoleThingy->AddLine("Usage: view <filename>", TatOSColors::Pinks::Pink);
        return;
    }

    File file;
    if (global_fs.ReadFile(Arguments, &file)) 
    {
        if (file.is_directory) 
        {
            ConsoleThingy->AddLine("Error: Target is a directory.", TatOSColors::Pinks::Pink);
            return;
        }

        // Output file contents to the console
        // (If file.data is a null-terminated text string)
        ConsoleThingy->AddLine(reinterpret_cast<const char*>(file.data), TatOSColors::Pinks::Pink);
    } else {
        ConsoleThingy->AddLine("File not found.", TatOSColors::Pinks::Pink);
    }
}