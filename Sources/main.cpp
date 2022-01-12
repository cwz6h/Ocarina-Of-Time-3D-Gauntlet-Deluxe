#include <3ds.h>
#include "csvc.h"
#include <CTRPluginFramework.hpp>

#include "cheats.hpp"
#include <vector>

namespace CTRPluginFramework
{
    // This patch the NFC disabling the touchscreen when scanning an amiibo, which prevents ctrpf to be used

        static MenuEntry *EntryWithHotkey(MenuEntry *entry, const std::vector<Hotkey> &hotkeys)
    {
        if (entry != nullptr)
        {
            for (const Hotkey &hotkey : hotkeys)
                entry->Hotkeys += hotkey;
        }

        return (entry);
    } 



    static void    ToggleTouchscreenForceOn(void)
    {
        static u32 original = 0;
        static u32 *patchAddress = nullptr;

        if (patchAddress && original)
        {
            *patchAddress = original;
            return;
        }

        static const std::vector<u32> pattern =
        {
            0xE59F10C0, 0xE5840004, 0xE5841000, 0xE5DD0000,
            0xE5C40008, 0xE28DD03C, 0xE8BD80F0, 0xE5D51001,
            0xE1D400D4, 0xE3510003, 0x159F0034, 0x1A000003
        };

        Result  res;
        Handle  processHandle;
        s64     textTotalSize = 0;
        s64     startAddress = 0;
        u32 *   found;

        if (R_FAILED(svcOpenProcess(&processHandle, 16)))
            return;

        svcGetProcessInfo(&textTotalSize, processHandle, 0x10002);
        svcGetProcessInfo(&startAddress, processHandle, 0x10005);
        if(R_FAILED(svcMapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, processHandle, (u32)startAddress, textTotalSize)))
            goto exit;

        found = (u32 *)Utils::Search<u32>(0x14000000, (u32)textTotalSize, pattern);

        if (found != nullptr)
        {
            original = found[13];
            patchAddress = (u32 *)PA_FROM_VA((found + 13));
            found[13] = 0xE1A00000;
        }

        svcUnmapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, textTotalSize);
exit:
        svcCloseHandle(processHandle);
    }

    // This function is called before main and before the game starts
    // Useful to do code edits safely
    void    PatchProcess(FwkSettings &settings)
    {
        ToggleTouchscreenForceOn();
    }

    // This function is called when the process exits
    // Useful to save settings, undo patchs or clean up things
    void    OnProcessExit(void)
    {
        ToggleTouchscreenForceOn();
    }

    void    InitMenu(PluginMenu &menu)
    {
        // Create your entries here, or elsewhere
        // You can create your entries whenever/wherever you feel like it
        
        // Example entry
        /*menu += new MenuEntry("Test", nullptr, [](MenuEntry *entry)
        {
            std::string body("What's the answer ?\n");

            body += std::to_string(42);

            MessageBox("UA", body)();
        });*/
    }

    

 int main()
    {
        if (Process::GetTitleID() == 0x0004000000033500)
        {
            PluginMenu *menu = new PluginMenu(Color::Silver << "Ocarina Of Time Gauntlet Deluxe", 1, 5, 2, "Plugin For OoT3D");
            menu->SynchronizeWithFrame(true);
            
            const std::vector<MenuEntry *> Items {

            new MenuEntry("Infinite Bombs", InfiniteBomb),
            new MenuEntry("Infinite Bombchu", InfiniteBombchu),
            new MenuEntry("Infinite Slingshot", InfiniteSlingshot),
            new MenuEntry("Infinite Deku Sticks", InfiniteStick),
            new MenuEntry("Infinite Arrows", InfiniteArrows),
            new MenuEntry("Infinite Deku Nuts", InfiniteNuts),
            new MenuEntry("Infinite Explosives", InfiniteExplosives),
            
            };

            menu->Append(new MenuFolder("Item Cheats", "Use Item Cheats", Items));

            
            const std::vector<MenuEntry *> Effects { 

            new MenuEntry("Inventory Modifier", nullptr, InventoryModifier),
            new MenuEntry("Invincible", Invincible),
            new MenuEntry("Refill Heart (Tap Hearts)", RefillHeart),
            new MenuEntry("Refill Magic (Tap Magic)", RefillLargeMagicbar),
            new MenuEntry("Infinite Rupees", MaxRupees),
            new MenuEntry("Collect Heart Pieces As Many Times As You Want", CollectHeartPiecesInOverworldAsMany),
            new MenuEntry("Have Nayru's Love Always Activated", AlwaysHaveNayrusLoveActivated),
            new MenuEntry("Use Any Item", UseAllItems),
            new MenuEntry("Open Chests As Many Times As You Want", OpenAnyChestInTheGameAsManyTimes),
            new MenuEntry("Infinite Sword Glitch", SwordGlitch),
            new MenuEntry("Deku Sticks Always On Fire", StickFire),

            };

            menu->Append(new MenuFolder("Effects", "Change Effects Of Link Or The Game", Effects));



            const std::vector<MenuEntry *> Gear {


            new MenuEntry("Unlock Kokiri Sword", UnlockKokiriSword),
            new MenuEntry("Unlock Master Sword", UnlockExcaliburSword),
            new MenuEntry("Unlock Giant's Knife", UnlockBiggoronSword),
            new MenuEntry("Unlock Deku Shield", UnlockWoodShield),
            new MenuEntry("Unlock Hylian Shield", UnlockHyruleShield),
            new MenuEntry("Unlock Mirror Shield", UnlockMirrorShield),

            };

            menu->Append(new MenuFolder("Gear", "Unlock Certain Gear", Gear));


            const std::vector<MenuEntry *> Movement {

            EntryWithHotkey(new MenuEntry("MoonJump (ZL)", MoonJump, "Fly"), { Hotkey(Key::ZL, "MoonJump (ZL)") }),
            EntryWithHotkey(new MenuEntry("Move Fast (ZR)", MoveFast, "Move Fast (ZR)"), { Hotkey(Key::ZR, "Move Fast (ZR)") }),
            };


            menu->Append(new MenuFolder("Movement", "Change Link's Movement", Movement));



            const std::vector<MenuEntry *> Size {

            new MenuEntry("Giant Link", GiantLink),
            new MenuEntry("Mini Link", MiniLink),
            new MenuEntry("Paper Link", PaperLink),
            new MenuEntry("Normal Link", NormalLink),
            };

            menu->Append(new MenuFolder("Size", "Change Link's Size", Size));




            const std::vector<MenuEntry *> Other {



            new MenuEntry("Have 100 Skulltulas", Skulltulas),
            new MenuEntry("Blue Gauntlet", BlueGauntlet),
            new MenuEntry("Green Gauntlet", GreenGauntlet),
            new MenuEntry("Purple Gauntlet", PurpleGauntlet),
            new MenuEntry("Black Gauntlet", BlackGauntlet),
            new MenuEntry("Always Adult Link Voice", AlwaysAdultLinkVoice),
            new MenuEntry("Always Young Link Voice", AlwaysChildLinkVoice),
	
		
	


            };
            menu->Append(new MenuFolder("Other", "Other Cheats", Other));
            
            


            menu->Run();
            delete menu;
        }
        else {
            MessageBox("This game is not supported.")();
            PluginMenu *menu = new PluginMenu("This Game Is Unsupported", 0, 0, 0, "Unsupported");
            menu->SynchronizeWithFrame(true);
            menu->Run();
            delete menu;
            return (0);
        }

        // Exit plugin
        
    }
}
    



                
                