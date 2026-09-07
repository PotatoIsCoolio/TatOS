/*
################################
#           [TatOS]            #
#      File: Console.cpp       #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include "Console.h"

void TatOSConsole::Init(TatOS::Screen* target_screen, uint32_t ScreenWidth, uint32_t ScreenHeight) 
{
    screen = target_screen;
    this->network = network;
    this->ScreenWidth = ScreenWidth;
    this->ScreenHeight = ScreenHeight;
    FontWidth = 8;
    LineHeight = ::FontHeight;
    CursorX = 0;
    CursorY = 0;
    InputLength = 0;
    input[0] = '\0';
    HistoryStart = 0;
    HistoryCount = 0;
    dirty = true;
}

void TatOSConsole::Clear() 
{
    HistoryStart = 0;
    HistoryCount = 0;
    dirty = true;
}

void TatOSConsole::PutChar(char c, uint32_t TextColor) 
{
    if (c == '\n') 
    {
        CursorX = TextPadding;
        CursorY += LineHeight + 2;
        return;
    }

    uint8_t char_w = font_widths[(uint8_t)c];
    uint32_t glyph_width = (char_w > 0 ? char_w : FontWidth);
    uint32_t right_edge = ScreenWidth > TextPadding ? ScreenWidth - TextPadding : ScreenWidth;

    if (right_edge <= TextPadding)
        return;

    if (CursorX + glyph_width + 1 > right_edge)
        PutChar('\n', TextColor);

    if (CursorY + LineHeight > ScreenHeight)
        return;

    screen->DrawChar(c, CursorX, CursorY, TextColor, TatOSColors::UI::ConsoleBg);
    CursorX += glyph_width + 1;
}

void TatOSConsole::Print(const char* str, uint32_t TextColor) 
{
    for (uint32_t i = 0; str[i] != '\0'; i++) 
    {
        PutChar(str[i], TextColor);
    }
}

void TatOSConsole::PrintLine(const char* str, uint32_t TextColor) 
{
    Print(str, TextColor);
    PutChar('\n', TextColor);
}

void TatOSConsole::AddLine(const char* str, uint32_t TextColor)
{
    uint32_t index;
    if (HistoryCount < HistoryCapacity)
    {
        index = (HistoryStart + HistoryCount) % HistoryCapacity;
        HistoryCount++;
    }
    else
    {
        index = HistoryStart;
        HistoryStart = (HistoryStart + 1) % HistoryCapacity;
    }

    uint32_t i = 0;
    while (str[i] != '\0' && i < LineCapacity - 1)
    {
        history[index][i] = str[i];
        i++;
    }

    history[index][i] = '\0';
    history_colors[index] = TextColor;
}

void TatOSConsole::AddPromptLine()
{
    char line[LineCapacity];
    const char* prompt = "TatOS> ";
    uint32_t i = 0;

    while (prompt[i] != '\0' && i < LineCapacity - 1)
    {
        line[i] = prompt[i];
        i++;
    }

    for (uint32_t j = 0; input[j] != '\0' && i < LineCapacity - 1; j++)
        line[i++] = input[j];

    line[i] = '\0';
    AddLine(line, TatOSColors::Blues::RoyalBlue);
}

void TatOSConsole::HandleKey(char key)
{
    if (key == '\b')
    {
        if (InputLength > 0)
        {
            InputLength--;
            input[InputLength] = '\0';
            dirty = true;
        }
        return;
    }

    if (key == '\n')
    {
        AddPromptLine();
        ExecuteCommand();
        InputLength = 0;
        input[0] = '\0';
        dirty = true;
        return;
    }

    if (key >= ' ' && key <= '~' && InputLength < InputCapacity - 1)
    {
        input[InputLength++] = key;
        input[InputLength] = '\0';
        dirty = true;
    }
}

bool TatOSConsole::NeedsRender() const
{
    return dirty;
}

void TatOSConsole::Render() 
{
    screen->Clear(TatOSColors::UI::ConsoleBg);
    CursorX = TextPadding;
    CursorY = TextPadding;

    PrintLine("TatOS Kernel Console Booted.", TatOSColors::Pinks::HotPink);
    PrintLine("Type 'help' to see available commands.", TatOSColors::Pinks::Pink);
    PutChar('\n', TatOSColors::Core::White);

    for (uint32_t i = 0; i < HistoryCount; i++)
    {
        uint32_t index = (HistoryStart + i) % HistoryCapacity;
        PrintLine(history[index], history_colors[index]);
    }

    Print("TatOS> ", TatOSColors::Blues::RoyalBlue);
    Print(input, TatOSColors::Core::White);
    dirty = false;
}
