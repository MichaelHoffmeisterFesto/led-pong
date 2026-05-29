#include "MenuGameSetup.h"
#include "MenuGameMain.h"
#include "MenuGameMsgBox.h"
#include "IntroGame.h"
#include "Networking.h"

// for linux diag
#ifdef WIN32
#else
#include <unistd.h>
#endif

MenuItem SetupMenu[] =
{
	/* 00 */ { MI_Switch  , "MUSIC ON" ,  0,  2, true , '+', ' ', 1, { 1 } },
	/* 01 */ { MI_Switch  , "MUSIC OFF",  0,  3, false, '+', ' ', 1, { 0 } },

	/* 02 */ { MI_Switch  , "NORM KEYS",  0,  5, true , '+', ' ', 1, { 3 } },
	/* 03 */ { MI_Switch  , "SWAP KEYS",  0,  6, false, '+', ' ', 1, { 2 } },

	/* 04 */ { MI_Button  , "LEAVE"    ,  0,  8, false, '+', ' ', 0, {} },

	/* right column */

	/* 05 */ { MI_TextOnly, "WIFI"     , 11,  2, false, '+', ' ', 0, {} },
	/* 06 */ { MI_Button  ,      "SRV" , 16,  2, false, '+', ' ', 0, {} },
	/* 07 */ { MI_Button  ,      "HKA" , 16,  3, false, '+', ' ', 0, {} },
	/* 08 */ { MI_Button  ,      "LHE" , 16,  4, false, '+', ' ', 0, {} },

	/* 09 */ { MI_Button  , "SYS-HALT" ,  11,  8, false, '+', ' ', 0, {} },
};

MenuGameSetup::MenuGameSetup(GameEnvironment* env)
	: MenuGameBase(env)
{
	SetupMenu[0].State = !Env->Mute;
	SetupMenu[1].State =  Env->Mute;

	SetupMenu[2].State = !Env->KeySwap;
	SetupMenu[3].State =  Env->KeySwap;

	LoadMenu(SetupMenu, SIZE_OF_ARR(SetupMenu), 0, "media/setup_4x3_small.bmp", Vec2(6, 12));
}

GameBase* MenuGameSetup::ButtonSelectLeft(int selectedItem)
{
	// start game by having a new object on the heap
	MenuGameMain* x = new MenuGameMain(Env);
	return x;
}

GameBase* MenuGameSetup::ButtonSelectRight(int selectedItem)
{
	if (selectedItem == 4)
	{
		// commit
		Env->Mute = mCurrMenu[1].State;
		Env->KeySwap = mCurrMenu[3].State;

		// start game by having a new object on the heap
		MenuGameMain* x = new MenuGameMain(Env);
		return x;
	}

	if (selectedItem == 6)
	{
		ExecScript("wifi-srv");

		// gather info
		Networking nw;
		nw.ReadAdapterInfo();

		// start game by having a new object on the heap
		string lines[] = {
				"AP   : LEDPONG",
				"PW   : HKA",
				"ETH0: 222.222.222.222",
				"WIFI: 188.245.234.234",
				"U    : STUDENT",
				"PW   : HKA",
		};
		MenuGameMsgBox* x = new MenuGameMsgBox(Env, "WIFI SERVER +++++++++++", lines, SIZE_OF_ARR(lines));
		return x;
	}

	if (selectedItem == 9)
	{
		ExecScript("halt");

		// start game by having a new object on the heap
		string lines[] = {
				"THE SYSTEM WILL HALT",
				"AS SOON AS POSSIBLE.",
				"",
				"THIS PROCESS CANT BE",
				"CANCELLED."
		};
		MenuGameMsgBox* x = new MenuGameMsgBox(Env, "SYSTEM ++++++++++++++++", lines, SIZE_OF_ARR(lines));
		return x;
	}

	//if (selectedItem >= 6 && selectedItem <= 9)
	//{
	//	string names[] = { "wifi-srv", "wifi-hka", "wifi-lhe", "halt" };
	//	ExecScript(names[selectedItem - 6]);
	//}

	return nullptr;
}

void MenuGameSetup::ExecScript(string name)
{
#ifdef WIN32
	string cmd = ".\\scripts\\" + name + ".bat";
	system(cmd.c_str());
	mSelectedItem = 4;
#else
	string cmd = "./scripts/" + name + ".sh";
	system(cmd.c_str());
	mSelectedItem = 4;
#endif
}

// Helps diagnosing if process is running with root privileges (0) or not
void MenuGameSetup::GidDiag()
{
#ifdef WIN32
#else
	std::cout << "getuid()  = " << getuid()  << std::endl;
    std::cout << "geteuid() = " << geteuid() << std::endl;
    std::cout << "id output:" << std::endl;
    system("id");

    std::cout << "geteuid() = " << geteuid() << std::endl;
    system("id");                 // zur Kontrolle
    int ret = system("/bin/bash /home/pi/led-pong/led-pong/src/LedPong/scripts/util_wifi_ap.sh");
    std::cout << "system() returned " << ret << std::endl;
#endif
}
