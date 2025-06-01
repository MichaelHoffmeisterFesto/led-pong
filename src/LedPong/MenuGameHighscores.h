#pragma once
#include "MenuGameBase.h"
class MenuGameHighscores : public MenuGameBase
{
public:
	MenuGameHighscores(GameEnvironment* env);
protected:
	virtual GameBase* ButtonSelectLeft(int selectedItem);
	virtual GameBase* ButtonSelectRight(int selectedItem);
};

