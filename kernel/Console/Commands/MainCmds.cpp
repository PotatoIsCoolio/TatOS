/*
################################
#           [TatOS]            #
#      File: MainCmds.cpp      #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "../Console.h"
#include <cstddef>

struct Command
{
    const char* Name;
    void (*Function)(TatOSConsole*, const char*);
};

// This is probably a really shitty way of doing it. But because theres only like 10 commands for now it will be fine.
void CmdHelp(TatOSConsole*, const char*);
void CmdStart(TatOSConsole*, const char*);
void CmdClear(TatOSConsole*, const char*);
void CmdAbout(TatOSConsole*, const char*);
void CmdVersion(TatOSConsole*, const char*);
void CmdEcho(TatOSConsole*, const char*);
void CmdLs(TatOSConsole* ConsoleThingy, const char* Arguments);
void CmdCat(TatOSConsole* ConsoleThingy, const char* Arguments);
void CmdUptime(TatOSConsole* console, const char*);
void CmdShutdown(TatOSConsole* console, const char*);
void CmdReboot(TatOSConsole* Console, const char*);
void CmdHWStats(TatOSConsole* ConsoleThingy, const char* Arguments);

size_t StrLen(const char* s)
{
    size_t len = 0;
    while (s[len]) len++;
    return len;
}

bool StrEqualN(const char* a, const char* b, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (a[i] != b[i]) return false;
    }
    return true;
}

Command commands[] =
{
    {"help", CmdHelp},
    {"start", CmdStart},
    {"clean", CmdClear},
    {"about", CmdAbout},
    {"version", CmdVersion},
    {"echo", CmdEcho},
    {"listdir", CmdLs},
    {"view", CmdCat},
    {"uptime", CmdUptime},
    {"shutdown", CmdShutdown},
    {"reboot", CmdReboot},
    {"hwstats", CmdHWStats}
};

void TatOSConsole::ExecuteCommand()
{
    const char* command = input;

    while (*command == ' ') 
        command++;

    if (*command == '\0') 
        return;

    const char* Arguments = command;

    while (*Arguments && *Arguments != ' ') 
        Arguments++;

    size_t len = Arguments - command;

    for (auto& cmd : commands)
    {
        if (StrEqualN(command, cmd.Name, len) && StrLen(cmd.Name) == len)
        {
            if (*Arguments == ' ') Arguments++; 
            cmd.Function(this, Arguments);
            return;
        }
    }

    AddLine("Unknown command. Please type in Help for more info.", TatOSColors::Reds::Red);
}