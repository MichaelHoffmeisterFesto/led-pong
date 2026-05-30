#pragma once
#include "MenuGameBase.h"
#include <functional>
class MenuGameMsgBox : public MenuGameBase
{
public:

	enum class Result { None, OK, Yes, No };
	enum class Buttons { OK, YesNo };

	MenuGameMsgBox(GameEnvironment* env, 
		string caption, string lines[], int numLine, Buttons buttons);

	std::function<GameBase*(Result r)> LambdaAction;

protected:
	Buttons mButtons = Buttons::OK;
	virtual GameBase* ButtonSelectLeft(int selectedItem);
	virtual GameBase* ButtonSelectRight(int selectedItem);
	MenuItem* items = nullptr;
	int numItems = 0;
};

