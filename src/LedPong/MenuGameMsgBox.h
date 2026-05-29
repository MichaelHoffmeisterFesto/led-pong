#pragma once
#include "MenuGameBase.h"
class MenuGameMsgBox : public MenuGameBase
{
public:
	MenuGameMsgBox(GameEnvironment* env, 
		string caption, string lines[], int numLine);
protected:
	virtual GameBase* ButtonSelectLeft(int selectedItem);
	virtual GameBase* ButtonSelectRight(int selectedItem);
	MenuItem* items = nullptr;
	int numItems = 0;
};

