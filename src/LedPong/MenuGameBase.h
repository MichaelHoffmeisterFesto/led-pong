#pragma once
#include <algorithm> // max, min
#include "GameConst.h"
#include "basic.h"
#include "LedAnimation.h"
#include "CQueueDynGeneric.h"

#include "GameConst.h"
#include "GameEnvironment.h"
#include "GameBase.h"

// some data structure
enum MenuItemKind { MI_TextOnly, MI_Switch, MI_Button, MI_EntryChar };

struct MenuItem
{
	MenuItemKind Kind;
	string Text;
	int X, Y;
	bool State;
	char OnChar = 'X', OffChar = ' ';
	int NumBrother = 0;
	int Brothers[3];
	bool NonPropText = false;
};

// This class implements a game menu
class MenuGameBase : public GameBase
{
public:
	MenuGameBase(GameEnvironment* env);
	~MenuGameBase();

	// Repeatedly called by the main application
	virtual void Loop();

protected:
	void LoadMenu(MenuItem* menu, int numItem, int startItem, string backgroundFn, Vec2 menuBasePos);
	bool MoveSelectedItem(int dir = 0);
	virtual GameBase* ButtonSelectLeft(int selectedItem);
	virtual GameBase* ButtonSelectRight(int selectedItem);

	// Ressources
	LedTexture mPageBackground;

	// other variables
	Vec2 mMenuBasePos;
	int mFirstFrames = GAME_FrameRate / 3;
	MenuItem* mCurrMenu = nullptr;
	int mNumItem = 0;
	int mSelectedItem = 0;
	GameBase* mChargeNextGame = nullptr;
	int mLastFrameIndexWithKey = 0;
};

