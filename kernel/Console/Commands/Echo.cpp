/*
################################
#           [TatOS]            #
#        File: Echo.cpp        #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "../Console.h"

void CmdEcho(TatOSConsole* console, const char* Arguments)
{
    while (*Arguments == ' ') 
        Arguments++;
        
    console->AddLine(Arguments, TatOSColors::Core::White);
}