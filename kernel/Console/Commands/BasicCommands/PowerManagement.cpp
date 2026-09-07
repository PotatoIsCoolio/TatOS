/*
################################
#           [TatOS]            #
#  File: PowerManagement.cpp   #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "../../Console.h"
#include "../../../Drivers/PowerManagement/Power.h"

void CmdShutdown(TatOSConsole* ConsoleThingy, const char*)
{
    Drivers::Power::Shutdown();
}

void CmdReboot(TatOSConsole* ConsoleThingy, const char*)
{
    Drivers::Power::Reboot();
}