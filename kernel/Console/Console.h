/*
################################
#           [TatOS]            #
#       File: Console.h        #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#pragma once
#include <stdint.h>
#include "../Drivers/Display/Handler.h"
#include "../Colors.h"

class TatOSNetwork;

class TatOSConsole 
{
    private:
        enum
        {
            InputCapacity = 64,
            HistoryCapacity = 20,
            LineCapacity = 80,
            TextPadding = 12
        };

        TatOS::Screen* screen;
        TatOSNetwork* network;
        uint32_t ScreenWidth;
        uint32_t ScreenHeight;
        uint32_t CursorX;
        uint32_t CursorY;
        uint32_t FontWidth;
        uint32_t LineHeight;

        char input[InputCapacity];
        uint32_t InputLength;

        char history[HistoryCapacity][LineCapacity];
        uint32_t history_colors[HistoryCapacity];
        uint32_t HistoryStart;
        uint32_t HistoryCount;
        bool dirty;
        bool GuiRequested = false;

        void PutChar(char c, uint32_t TextColor);
        void Print(const char* str, uint32_t TextColor);
        void PrintLine(const char* str, uint32_t TextColor);
        void AddPromptLine();
        void ExecuteCommand();

    public:
        void Init(TatOS::Screen* Screen, uint32_t ScreenWidth, uint32_t ScreenHeight);
        void Clear();
        void HandleKey(char key);
        bool NeedsRender() const;
        void Render();
        bool ShouldStartGUI() const 
        {
            return GuiRequested; 
        }
        void RequestGUI() {GuiRequested = true;}

        void AddLine(const char* str, uint32_t TextColor);
};


namespace
{
    // I also have on in my file system. but that works a little differently.
    bool StringsEqual(const char* left, const char* right)
    {
        while (*left != '\0' && *right != '\0')
        {
            if (*left != *right)
                return false;

            left++;
            right++;
        }

        return *left == *right;
    }

    bool StartsWith(const char* text, const char* prefix)
    {
        while (*prefix != '\0')
        {
            if (*text++ != *prefix++)
                return false;
        }

        return true;
    }
}