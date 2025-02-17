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
enum MenuItemKind { MI_TextOnly, MI_Switch, MI_Button };

struct MenuItem
{
	MenuItemKind Kind;
	string Text;
	int X, Y;
	bool State;
	char OnChar = 'X', OffChar = ' ';
	int NumBrother = 0;
	int Brothers[3];
};

// This class implements a game menu
class MenuGame : public GameBase
{
public:
	MenuGame(GameEnvironment* env, int menuIndex = -1);
	~MenuGame();

	// select menue
	void LoadMenu(int menuIndex);

	// Repeatedly called by the main application
	virtual void Loop();

protected:
	bool MoveSelectedItem(int dir = 0);

private:
	// Ressources
	LedTexture PageMainMenu;

	// other variables
	int mFirstFrames = GAME_FrameRate / 3;
	int mLoadIndex = 0;
	MenuItem* mCurrMenu = nullptr;
	int mNumItem = 0;
	int mSelectedItem = 0;
	GameBase* mChargeNextGame = nullptr;
	int mLastFrameIndexWithKey = 0;
};

