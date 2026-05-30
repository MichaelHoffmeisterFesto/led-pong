#include "MenuGameMsgBox.h"
#include "MenuGameMain.h"

MenuGameMsgBox::MenuGameMsgBox(GameEnvironment* env, 
	string caption, string lines[], int numLine, Buttons buttons) : MenuGameBase(env)
{
	// allocate items
	mButtons = buttons;
	numItems = 2 + numLine;
	if (mButtons == Buttons::YesNo)
		numItems++;
	items = new MenuItem[numItems];

	// fix items
	items[0] = { MI_TextOnly  , "" + caption ,  0,  0, false , '+', ' ', 0, {} };

	int startItem = 0;
	if (mButtons == Buttons::YesNo)
	{
		// YesNo
		items[numItems - 2] = { MI_Button  , "NO"    ,   0,  8, false, '+', ' ', 0, {} };
		items[numItems - 1] = { MI_Button  , "YES"   ,  15,  8, false, '+', ' ', 0, {} };
		startItem = numItems - 2;
	}
	else
	{
		// default
		items[numItems - 1] = { MI_Button  , "OK"    ,  15,  8, false, '+', ' ', 0, {} };
		startItem = numItems - 1;
	}

	// variable items
	for (int i=0; i<numLine; i++)
		items[1+i] = { MI_TextOnly  , "" + lines[i] ,  0,  2+i, false , '+', ' ', 0, {}};

	// load
	LoadMenu(items, numItems, 0, "media/msgbox_4x3_small.bmp", Vec2(6, 12));
	mSelectedItem = startItem;
}

GameBase* MenuGameMsgBox::ButtonSelectLeft(int selectedItem)
{
	return new MenuGameMain(Env);
}

GameBase* MenuGameMsgBox::ButtonSelectRight(int selectedItem)
{
	Result processResult = Result::None;

	if (mButtons == Buttons::YesNo)
	{
		// YesNo
		if (selectedItem == numItems - 1)
		{
			processResult = Result::Yes;
		}
		if (selectedItem == numItems - 2)
		{
			processResult = Result::No;
		}
	}
	else
	{
		// default
		if (selectedItem == numItems - 1)
		{
			processResult = Result::OK;
		}
	}

	if (processResult != Result::None)
	{
		// either default action (main menu) or lambda action
		if (LambdaAction == nullptr)
		{
			return new MenuGameMain(Env);
		}
		else
		{
			// evaluate lambda
			return LambdaAction(processResult);
		}
	}

	return nullptr;
}
