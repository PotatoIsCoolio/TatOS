/*
################################
#           [TatOS]            #
#      File: Version.cpp       #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "../../Console.h"

void CmdVersion(TatOSConsole* console, const char*)
{
    // Ill add a actual "version" logic later.
    console->AddLine("TatOS kernel!! This was the first ever command created.", TatOSColors::Cyans::Cyan);
}