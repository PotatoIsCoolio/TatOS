/*
################################
#           [TatOS]            #
#       File: clear.cpp        #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "../../Console.h"

void CmdClear(TatOSConsole* ConsoleThingy, const char*)
{
    ConsoleThingy->Clear();
}
