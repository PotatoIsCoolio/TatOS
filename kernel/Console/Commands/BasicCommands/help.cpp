/*
################################
#           [TatOS]            #
#        File: help.cpp        #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "../../Console.h"

void CmdHelp(TatOSConsole* console, const char*)
{
    // I tried to make the Text line up in the Terminal but unable to get it lined up. 
    // Its not that big of a deal. So I dont really mind it
    console->AddLine("Commands:", TatOSColors::Pinks::HotPink);
    console->AddLine("  help             Show this list", TatOSColors::Core::White);
    console->AddLine("  start            Starts the GUI version", TatOSColors::Core::White);
    console->AddLine("  clean            Clear the console", TatOSColors::Core::White);
    console->AddLine("  about            Show TatOS information", TatOSColors::Core::White);
    console->AddLine("  version          Show the console version", TatOSColors::Core::White);
    console->AddLine("  echo <text>      Print text", TatOSColors::Core::White);
    console->AddLine("  listdir          Lists all the current files in the DIR", TatOSColors::Core::White);
    console->AddLine("  view             Views data of a current file.", TatOSColors::Core::White);
    console->AddLine("  uptime           Show system uptime", TatOSColors::Core::White);
    console->AddLine("  shutdown         Shuts down the operating system", TatOSColors::Core::White);
    console->AddLine("  reboot           Reboots the operating system", TatOSColors::Core::White);
    console->AddLine("  hwstats          Lists system hardware", TatOSColors::Core::White);
    return;
}