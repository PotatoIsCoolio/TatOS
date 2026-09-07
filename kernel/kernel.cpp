/*
################################
#           [TatOS]            #
#       File: kernel.cpp       #
#     Hobby OS developed by:   #
#        @PotatoIsCool         #
################################
*/
#include <stdint.h>
#include "Kernel.h"
#include "Colors.h"
#include "Filesystem/TatoFS.h"
#include "Console/Console.h"

#include "Drivers/IDT/IDT.h"
#include "Drivers/Timer/Timer.h"
#include "Drivers/Keyboard/Input/PS2Input.h"
#include "Drivers/Keyboard/Handler.h"
#include "Drivers/HardwareManager/SystemInfo.h"
#include "Drivers/Display/Handler.h"

void OnPITInterrupt(Registers* regs) 
{
    Drivers::PIT::OnTick();
}

extern "C" 
{
    __attribute__((section(".multiboot"), used))
    const uint32_t multiboot_header[] = {MULTIBOOT_MAGIC, MULTIBOOT_FLAGS, MULTIBOOT_CHECKSUM, 0, 0, 0, 0, 0, 0, 1920, 1080, 128};
    extern const uint8_t _binary_initrd_tar_start[];
    extern const uint8_t _binary_initrd_tar_end[];
}

MultibootInfo* MultibootInfoStat = nullptr;
uint32_t MultibootMagicStat = 0;

extern "C" void KernelMain(uint32_t magic, TatOS::Multiboot::MultibootInfo* MultibootInfo)
{
    if (magic != 0x2BADB002 || !(MultibootInfo->flags & (1 << 12))) 
        return;

    MultibootMagicStat = magic;
    TatOS::Multiboot::MultibootInfo* MultibootInfoStat;

    uintptr_t initrd_start = reinterpret_cast<uintptr_t>(_binary_initrd_tar_start);
    uintptr_t initrd_end = reinterpret_cast<uintptr_t>(_binary_initrd_tar_end);
    global_fs.init(initrd_start, initrd_end);

    TatOS::IDT::Interrupts::Initialize();
    TatOS::IDT::Interrupts::RegisterHandler(32, OnPITInterrupt);
    Drivers::PIT::Initialize(1000);
    //asm volatile("int $0x03");
    TatOS::IDT::Interruptions(true); // Enables Interruptions
    //DisableInterruptions();

    TatOS::Screen UsersScreen;
    TatOSConsole console;
    TatOS::Keyboard Keyboard;
    TatOS::Mouse mouse;
    TatOS::PS2Input input;

    UsersScreen.Init(MultibootInfo);
    Keyboard.InitKeyboard();
    mouse.Init(MultibootInfo);

    console.Init(&UsersScreen, MultibootInfo->framebuffer_width, MultibootInfo->framebuffer_height);
    console.Render();
    UsersScreen.SwapBuffers();

    while (!console.ShouldStartGUI()) 
    {
        char key;
        while (input.Poll(Keyboard, mouse, key))
        {
            if (key != 0)
                console.HandleKey(key);
        }

        if (console.NeedsRender())
        {
            console.Render();
            UsersScreen.SwapBuffers();
        }
    }

    UsersScreen.Clear(TatOSColors::UI::DesktopBg);
    
    uint32_t CenterX = MultibootInfo->framebuffer_width / 2;
    uint32_t CenterY = MultibootInfo->framebuffer_height / 2;

    int32_t LastPosX = mouse.state.x;
    int32_t LastPosY = mouse.state.y;
    bool LastClick = mouse.state.left_click;

    
    LoadedImage KittyImage = UsersScreen.LoadImageFromFile("Kitty.png");
    UsersScreen.DrawImage(KittyImage, CenterX - 150, 100, 300, 300);
    
    UsersScreen.DrawString("TatOS Desktop Testing", 10, 10, TatOSColors::Core::White, TatOSColors::UI::DesktopBg);
    UsersScreen.FillRect(CenterX - 20, CenterY - 20, 40, 40, TatOSColors::Pinks::HotPink);
    UsersScreen.DrawCursor(mouse.state.x, mouse.state.y, mouse.state.left_click);
    UsersScreen.SwapBuffers();

    while (1) 
    {
        mouse.Poll();
        Drivers::PIT::SleepMicroseconds(1);
        if (mouse.state.x != LastPosX || mouse.state.y != LastPosY || mouse.state.left_click != LastClick) 
        {
            UsersScreen.Clear(TatOSColors::UI::DesktopBg);
            UsersScreen.DrawImage(KittyImage, CenterX - 150, 100, 300, 300); // Causes lag due to drawing the image a million times per tick or whatever. WILL FIX LATER
            UsersScreen.DrawString("TatOS Desktop Testing", 10, 10, TatOSColors::Core::White, TatOSColors::UI::DesktopBg);
            UsersScreen.FillRect(CenterX - 20, CenterY - 20, 40, 40, TatOSColors::Pinks::HotPink);

            UsersScreen.DrawCursor(mouse.state.x, mouse.state.y, mouse.state.left_click);

            UsersScreen.SwapMouseRegion(LastPosX, LastPosY);         
            UsersScreen.SwapMouseRegion(mouse.state.x, mouse.state.y); 

            LastPosX = mouse.state.x;
            LastPosY = mouse.state.y;
            LastClick = mouse.state.left_click;   
        }
    }
}