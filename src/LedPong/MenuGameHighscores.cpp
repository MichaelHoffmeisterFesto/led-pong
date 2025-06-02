#include <cassert>

#include "MenuGameHighscores.h"
#include "MenuGameMain.h"
#include "IntroGame.h"
#include "PacManGame.h"

MenuItem HighscoreMenu[] =
{
	/* 00 */ { MI_TextOnly , "++ PLAYER 1 WON ++" ,  2,  2, false , '+', ' ', 0, {}, false, LedColor(0x9f9f9f) },

	/* 01 */ { MI_TextOnly , "  1  FIL   0100000" ,  0,  3, false , '+', ' ', 0, {}, true , LedColor(0x3fff3f) },
	/* 02 */ { MI_TextOnly , "  2  SMM   0080000" ,  0,  4, false , '+', ' ', 0, {}, true , LedColor(0xff3f3f) },
																						  
	/* 03 */ { MI_TextOnly , "  3 +"              ,  0,  5, false , '+', ' ', 0, {}, true  },
	/* 04 */ { MI_EntryChar, "A"                  ,  5,  5, false , '+', ' ', 0, {}, true  },
	/* 05 */ { MI_EntryChar, "A"                  ,  6,  5, false , '+', ' ', 0, {}, true  },
	/* 06 */ { MI_EntryChar, "A"                  ,  7,  5, false , '+', ' ', 0, {}, true  },
	/* 07 */ { MI_TextOnly, "+  0078886"          ,  8,  5, false , '+', ' ', 0, {}, true  },
																						  
	/* 08 */ { MI_TextOnly , "  4  MLK   0053234" ,  0,  6, false , '+', ' ', 0, {}, true , LedColor(0xffff3f) },
	/* 09 */ { MI_TextOnly , "  5  LOV   0033000" ,  0,  7, false , '+', ' ', 0, {}, true , LedColor(0xff3fff) },

	/* 10 */ { MI_Button   , "RESTART"    ,  3,  8, false, '+', ' ', 0, {} },
	/* 11 */ { MI_Button   , "MAIN"       , 12,  8, false, '+', ' ', 0, {} },
};

LedColor PresetColors[] = {
	LedColor(0x3fff3f),
	LedColor(0xff3f3f),
	LedColor(0xffff3f),
	LedColor(0xff3fff)
};

MenuGameHighscores::MenuGameHighscores(GameEnvironment* env, int playerNum, int score)
	: MenuGameBase(env)
{
	// insert the new score
	mNewScoreIndex = InsertScoreGiveIndex(score);

	// load this as a template
	LoadMenu(HighscoreMenu, SIZE_OF_ARR(HighscoreMenu), 6, "media/highscores_4x3_small.bmp", Vec2(6, 14));

	// overwrite the (copied, loaded) lines from above
	char buffer[100];
	sprintf(buffer, "++ PLAYER %1d WON ++", 1 + playerNum);
	mCurrMenu[0].Text = buffer;

	// Note: the following code completely relies on the fact, that
	// the menu items are already created properly by the global array!
	int midx = 1; int hidx = 0; int hnum = 0;
	while (hidx < mNewScoreIndex && hnum < 4)
	{
		sprintf(buffer, "%3d  %3s  %07d", 1 + hidx, Env->HighScore[hidx].Name, Env->HighScore[hidx].Score);
		MenuItem mi {
			MI_TextOnly,
			std::string(buffer),
			0, 3 + hnum, false, '+', ' ', 0, {}, true,
			PresetColors[hnum % SIZE_OF_ARR(PresetColors)]
		};
		mCurrMenu[midx++] = mi;
		hidx++; hnum++;
	}

	// fill the active line
	hidx = mNewScoreIndex;
	sprintf(buffer, "%3d +", 1 + hidx);
	{
		MenuItem mi {
			MI_TextOnly,
			std::string(buffer),
			0, 3 + hnum, false, '+', ' ', 0, {}, true
		};
		mCurrMenu[midx++] = mi;
	}

	mSelectedItem = midx;
	mFirstEntryItem = midx;
	for (int i = 0; i < 3; i++)
	{
		MenuItem mi {
			MI_EntryChar,
			"A",
			5 + i, 3 + hnum, false, '+', ' ', 0, {}, true
		};
		mCurrMenu[midx++] = mi;
	}

	sprintf(buffer, "+ %07d", Env->HighScore[hidx].Score);
	{
		MenuItem mi{
			MI_TextOnly,
			std::string(buffer),
			8, 3 + hnum, false, '+', ' ', 0, {}, true
		};
		mCurrMenu[midx++] = mi;
	}
	hidx++; hnum++;

	// further lines?
	while (hnum < 5)
	{
		sprintf(buffer, "%3d  %3s  %07d", 1 + hidx, Env->HighScore[hidx].Name, Env->HighScore[hidx].Score);
		MenuItem mi {
			MI_TextOnly,
			std::string(buffer),
			0, 3 + hnum, false, '+', ' ', 0, {}, true,
			PresetColors[hnum % SIZE_OF_ARR(PresetColors)]
		};
		mCurrMenu[midx++] = mi;
		hidx++; hnum++;
	}

	if (midx != 10)
	{
		cerr << "MenuGameHighscores::Wrong number of menu items!!" << endl;
		assert(false);
	}
}

GameBase* MenuGameHighscores::ButtonSelectLeft(int selectedItem)
{
	// will always require leave!
	return nullptr;
}

GameBase* MenuGameHighscores::ButtonSelectRight(int selectedItem)
{
	if ( (selectedItem == 10 || selectedItem == 11)
		 && mFirstEntryItem >= 0 && mFirstEntryItem < 10 - 2
	     && mNewScoreIndex >= 0 && mNewScoreIndex < GAME_Max_Highscore)
	{
		for (int i=0; i<3; i++)
			Env->HighScore[mNewScoreIndex].Name[i] = mCurrMenu[mFirstEntryItem + i].Text.at(0);
	}

	if (selectedItem == 10)
	{
		return new PacManGame(Env, Env->LastGamePlayers, Env->LastGameDiffi);
	}

	if (selectedItem == 11)
	{
		return new MenuGameMain(Env);
	}

	return nullptr;
}

int MenuGameHighscores::InsertScoreGiveIndex(int score)
{
	// correct
	if (score <= 0)
		score = 1;

	// find the number of high scores, which are ABOVE the score
	int abv = 0;
	while (Env->HighScoreNum > abv && Env->HighScore[abv].Score > score)
		abv++;

	// the score shall always fir into the first MAX enxtries!
	if (abv >= GAME_Max_Highscore)
		abv = GAME_Max_Highscore - 1;

	// shift entries below one further
	for (int i = GAME_Max_Highscore - 1; i >= abv + 1; i--)
		Env->HighScore[i] = Env->HighScore[i - 1];

	// set at <abv> position
	Env->HighScore[abv] = { "AAA", score };

	// may be increase the number
	Env->HighScoreNum = std::min(GAME_Max_Highscore - 1, 1 + Env->HighScoreNum);

	// give back index
	return abv;
}
