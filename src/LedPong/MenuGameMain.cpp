#include "MenuGameMain.h"

#include "IntroGame.h"
#include "PacManGame.h"
#include "MenuGameSetup.h"

MenuItem GlobalMenu[] =
{
	/* 00 */ { MI_TextOnly, "MAIN MENU",  0,  0, false, ' ', ' ', 0, {} },
	/* 01 */ { MI_Button  , "START"    ,  0,  2, false, '+', ' ', 0, {} },
	/* 02 */ { MI_Switch  , "1 PLAYER" ,  9,  1, true , '+', ' ', 1, { 3 } },
	/* 03 */ { MI_Switch  , "2 PLAYER" ,  9,  2, false, '+', ' ', 1, { 2 } },
	/* 04 */ { MI_Switch  , "ROOKIE"   , 10,  6, true, '+', ' ', 2, { 5, 6 } },
	/* 05 */ { MI_Switch  , "NORMAL"   , 10,  7, false,  '+', ' ', 2, { 4, 6 } },
	/* 06 */ { MI_Switch  , "EXPERT"   , 10,  8, false, '+', ' ', 2, { 4, 5 } },
	/* 07 */ { MI_Button  , "SETUP"    , 10,  9, false, '+', ' ', 0, {} },
};

MenuGameMain::MenuGameMain(GameEnvironment* env)
	: MenuGameBase(env)
{
	LoadMenu(GlobalMenu, SIZE_OF_ARR(GlobalMenu), 1, "media/ai_tech_pac_4x3_small.bmp", Vec2(6, 7));
}

GameBase* MenuGameMain::ButtonSelectLeft(int selectedItem)
{
	return new IntroGame(Env);
}

GameBase* MenuGameMain::ButtonSelectRight(int selectedItem)
{
	if (selectedItem == 1)
	{
		// get the mode
		Env->LastGamePlayers = (mCurrMenu[3].State == true) ? 2 : 1;
		Env->LastGameDiffi = 1;
		if (mCurrMenu[5].State == true)
			Env->LastGameDiffi = 2;
		if (mCurrMenu[6].State == true)
			Env->LastGameDiffi = 3;

		// start game!!
		return new PacManGame(Env, Env->LastGamePlayers, Env->LastGameDiffi);
	}

	if (selectedItem == 7)
	{
		return new MenuGameSetup(Env);
	}

	return nullptr;
}
