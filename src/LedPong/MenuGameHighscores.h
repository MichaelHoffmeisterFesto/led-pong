#pragma once
#include "MenuGameBase.h"
class MenuGameHighscores : public MenuGameBase
{
public:
	MenuGameHighscores(GameEnvironment* env, int playerNum, int score);
protected:
	virtual GameBase* ButtonSelectLeft(int selectedItem);
	virtual GameBase* ButtonSelectRight(int selectedItem);

	int InsertScoreGiveIndex(int score);

	int mNewScoreIndex = 0;
	int mFirstEntryItem = 0;
};

