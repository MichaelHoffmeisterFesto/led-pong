#include "MenuGameSetup.h"
#include "MenuGameMain.h"
#include "IntroGame.h"

MenuItem SetupMenu[] =
{
	/* 00 */ { MI_Switch  , "MUSIC ON" ,  0,  2, true , '+', ' ', 1, { 1 } },
	/* 01 */ { MI_Switch  , "MUSIC OFF",  0,  3, false, '+', ' ', 1, { 0 } },

	/* 02 */ { MI_Switch  , "NORM KEYS",  0,  5, true , '+', ' ', 1, { 3 } },
	/* 03 */ { MI_Switch  , "SWAP KEYS",  0,  6, false, '+', ' ', 1, { 2 } },

	/* 04 */ { MI_Button  , "LEAVE"    ,  0,  8, false, '+', ' ', 0, {} },
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
	return new MenuGameMain(Env);
}

GameBase* MenuGameSetup::ButtonSelectRight(int selectedItem)
{
	if (selectedItem == 4)
	{
		// commit
		Env->Mute = mCurrMenu[1].State;
		Env->KeySwap = mCurrMenu[3].State;

		return new MenuGameMain(Env);
	}

	return nullptr;
}
