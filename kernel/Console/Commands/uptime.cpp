/*
################################
#           [TatOS]            #
#       File: uptime.cpp       #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "../Console.h"
#include "../../Drivers/Timer/Timer.h"

// Not made by Potato
static void StringCopy(char* dest, const char* src) 
{
    while (*src) *dest++ = *src++;
    *dest = '\0';
}
// Not made by Potato
static void StringAppend(char* dest, const char* src) 
{
    while (*dest) dest++;
    while (*src) *dest++ = *src++;
    *dest = '\0';
}
// Not made by Potato
static void UIntToString(uint32_t val, char* buf) {
    if (val == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    int i = 0;
    char temp[12];
    while (val > 0) {
        temp[i++] = (val % 10) + '0';
        val /= 10;
    }
    int j = 0;
    while (i > 0) {
        buf[j++] = temp[--i];
    }
    buf[j] = '\0';
}

void CmdUptime(TatOSConsole* console, const char*)
{
    uint32_t TotalTicks = (uint32_t)Drivers::PIT::GetTicks(); 
    uint32_t TotalSeconds = TotalTicks / 1000;
    
    uint32_t hours = (TotalSeconds / 3600);
    uint32_t minutes = (TotalSeconds % 3600) / 60;
    uint32_t seconds = (TotalSeconds % 60);
    uint32_t ms = (TotalTicks % 1000);

    char LineBuffer[128];
    char num_buf[16];

    StringCopy(LineBuffer, "TatOS Kernel Uptime: ");

    if (hours > 0) 
    {
        UIntToString(hours, num_buf);
        StringAppend(LineBuffer, num_buf);
        StringAppend(LineBuffer, "h ");
    }
    if (minutes > 0 || hours > 0) 
    {
        UIntToString(minutes, num_buf);
        StringAppend(LineBuffer, num_buf);
        StringAppend(LineBuffer, "m ");
    }

    UIntToString(seconds, num_buf);
    StringAppend(LineBuffer, num_buf);
    StringAppend(LineBuffer, ".");

    if (ms < 100) 
        StringAppend(LineBuffer, "0");

    if (ms < 10)  
        StringAppend(LineBuffer, "0");

    UIntToString(ms, num_buf);
    StringAppend(LineBuffer, num_buf);
    StringAppend(LineBuffer, "s");

    console->AddLine(LineBuffer, TatOSColors::Core::White);
}