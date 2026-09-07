/*
################################
#           [TatOS]            #
#       File: About.cpp        #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "../../Console.h"

void CmdAbout(TatOSConsole* ConsoleThingy, const char*)
{
    ConsoleThingy->AddLine("Bleh TatOS Testing.", TatOSColors::Pinks::Pink);
}
