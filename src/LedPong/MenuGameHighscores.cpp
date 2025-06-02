#include "MenuGameHighscores.h"
#include "MenuGameMain.h"
#include "IntroGame.h"

MenuItem HighscoreMenu[] =
{
	/* 00 */ { MI_TextOnly , "++ PLAYER 1 WON ++",  0,  2, false , '+', ' ', 0, {} },

	/* 01 */ { MI_TextOnly , "1    FIL   0100000" ,  0,  3, false , '+', ' ', 0, {}, true },
	/* 02 */ { MI_TextOnly , "2    SMM   0080000" ,  0,  4, false , '+', ' ', 0, {}, true },

	/* 03 */ { MI_TextOnly , "3   +"              ,  0,  5, false , '+', ' ', 0, {}, true },
	/* 04 */ { MI_EntryChar, "A"                  ,  5,  5, false , '+', ' ', 0, {}, true },
	/* 05 */ { MI_EntryChar, "A"                  ,  6,  5, false , '+', ' ', 0, {}, true },
	/* 06 */ { MI_EntryChar, "A"                  ,  7,  5, false , '+', ' ', 0, {}, true },
	/* 07 */ { MI_TextOnly, "+  0078886"          ,  8,  5, false , '+', ' ', 0, {}, true },

	/* 08 */ { MI_TextOnly , "4    MLK   0053234" ,  0,  6, false , '+', ' ', 0, {}, true },
	/* 09 */ { MI_TextOnly , "5    LOV   0033000" ,  0,  7, false , '+', ' ', 0, {}, true },

	/* 10 */ { MI_Button   , "RESTART"    ,  0,  8, false, '+', ' ', 0, {} },
	/* 11 */ { MI_Button   , "MAIN"       , 10,  8, false, '+', ' ', 0, {} },
};

MenuGameHighscores::MenuGameHighscores(GameEnvironment* env)
	: MenuGameBase(env)
{
	LoadMenu(HighscoreMenu, SIZE_OF_ARR(HighscoreMenu), 4, "media/highscores_4x3_small.bmp", Vec2(6, 12));
}

GameBase* MenuGameHighscores::ButtonSelectLeft(int selectedItem)
{
	// will always require leave!
	return nullptr;
}

GameBase* MenuGameHighscores::ButtonSelectRight(int selectedItem)
{
	if (selectedItem == 99)
	{
		return new MenuGameMain(Env);
	}

	return nullptr;
}
