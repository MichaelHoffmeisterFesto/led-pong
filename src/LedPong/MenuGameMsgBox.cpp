#include "MenuGameMsgBox.h"
#include "MenuGameMain.h"

MenuGameMsgBox::MenuGameMsgBox(GameEnvironment* env, 
	string caption, string lines[], int numLine) : MenuGameBase(env)
{
	// allocate items
	numItems = 2 + numLine;
	items = new MenuItem[numItems];

	// fix items
	items[0] = { MI_TextOnly  , "" + caption ,  0,  0, false , '+', ' ', 0, {} };
	items[numItems - 1] = { MI_Button  , "OK"    ,  15,  8, false, '+', ' ', 0, {} };

	// variable items
	for (int i=0; i<numLine; i++)
		items[1+i] = { MI_TextOnly  , "" + lines[i] ,  0,  2+i, false , '+', ' ', 0, {}};

	// load
	LoadMenu(items, numItems, 0, "media/msgbox_4x3_small.bmp", Vec2(6, 12));
	mSelectedItem = numItems - 1;
}

GameBase* MenuGameMsgBox::ButtonSelectLeft(int selectedItem)
{
	return new MenuGameMain(Env);
}

GameBase* MenuGameMsgBox::ButtonSelectRight(int selectedItem)
{
	if (selectedItem == numItems - 1)
	{
		return new MenuGameMain(Env);
	}

	return nullptr;
}
