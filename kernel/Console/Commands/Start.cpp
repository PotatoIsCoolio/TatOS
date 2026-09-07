/*
################################
#           [TatOS]            #
#       File: Start.cpp        #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "../Console.h"

void CmdStart(TatOSConsole* console, const char*)
{
    console->AddLine("Starting TatOS...", TatOSColors::Pinks::HotPink);
    console->RequestGUI();
    return; // Needed to add this else the console re-draws
}
