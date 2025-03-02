#include <algorithm> // max, min
#include "basic.h"
#include "LedAnimation.h"
#include "CQueueDynGeneric.h"

#include "GameConst.h"
#include "GameBase.h"
#include "MenuGame.h"

#include "IntroGame.h"
#include "PacManGame.h"

MenuItem NullMenu[]{
	/* 00 */ { MI_TextOnly, "", 0,  0, false, ' ', ' ', 0, {} },
};

MenuItem GlobalMenu[] =
{
	/* 00 */ { MI_TextOnly, "MAIN MENU",  0,  0, false, ' ', ' ', 0, {} },
	/* 01 */ { MI_Switch  , "1 PLAYER" ,  0,  2, true , '+', ' ', 1, { 2 } },
	/* 02 */ { MI_Switch  , "2 PLAYER" ,  0,  3, false, '+', ' ', 1, { 1 } },
	/* 03 */ { MI_Switch  , "ROOKIE"   ,  0,  5, false, '+', ' ', 2, { 4, 5 } },
	/* 04 */ { MI_Switch  , "NORMAL"   ,  0,  6, true,  '+', ' ', 2, { 3, 5 } },
	/* 05 */ { MI_Switch  , "EXPERT"   ,  0,  7, false, '+', ' ', 2, { 3, 4 } },
	/* 06 */ { MI_Button  , "START"    ,  0,  9, false, '+', ' ', 0, {} },
	/* 07 */ { MI_Button  , "SETUP"    , 10,  9, false, '+', ' ', 0, {} },
};

void MenuGame::LoadMenu(int menuIndex)
{
	// delete old menu structure?
	if (mCurrMenu != nullptr)
	{
		delete mCurrMenu;
		mCurrMenu = nullptr;
	}

	// load new
	mLoadIndex = menuIndex;
	int len = 0;
	switch (menuIndex)
	{
		case 0:
			len = sizeof(GlobalMenu);
			mCurrMenu = (MenuItem*) malloc(len);
			memcpy_s(mCurrMenu, len, GlobalMenu, sizeof(GlobalMenu));
			mNumItem = SIZE_OF_ARR(GlobalMenu);
			mSelectedItem = 1;
			break;
		
		default:
			break;
	}
}

MenuGame::MenuGame(GameEnvironment* env, int menuIndex) : GameBase(env)
{
	// Ressources
	PageMainMenu = LedTexture("media/main_menu.bmp");

	// load
	if (menuIndex >= 0)
		LoadMenu(menuIndex);
}

void RenderMenu(GameEnvironment* env, Vec2 startPos, MenuItem* menu, int numItem, int selectedItem, int extraRenderSpacingX)
{
	char buffer[80];

	for (int i = 0; i < numItem; i++)
	{
		// eval state
		MenuItem* mi = &menu[i];
		strcpy_s(buffer, "");
		switch (mi->Kind)
		{
			case MI_Switch:
				sprintf_s(buffer, "%c %s", mi->State ? mi->OnChar : mi->OffChar, mi->Text.c_str());
				break;

			default:
				strcpy_s(buffer, mi->Text.c_str());
				break;
		}

		// draw text
		Vec2 pos = startPos + Vec2(mi->X * 5, mi->Y * 9);
		env->TrPt.DrawTextTo(env->Screen, pos, buffer, 1 + extraRenderSpacingX, 1, 128);

		// selected?
		if (i == selectedItem)
		{
			env->Screen.DrawRect(LedColors.White, pos.X - 2, pos.Y - 2, 2 + 6 * (0 + strnlen_s(buffer, sizeof(buffer))), 8);
		}
	}
}

MenuGame::~MenuGame()
{
	// delete old menu structure?
	if (mCurrMenu != nullptr)
	{
		delete mCurrMenu;
		mCurrMenu = nullptr;
	}
}


bool MenuGame::MoveSelectedItem(int dir)
{
	int i = mSelectedItem;
	if (dir == -1)
		while (i > 0) {
			i--;
			if (mCurrMenu[i].Kind != MI_TextOnly)
			{
				mSelectedItem = i;
				return true;
			}
		}
	else if (dir == +1)
		while (i < mNumItem - 1) {
			i++;
			if (mCurrMenu[i].Kind != MI_TextOnly)
			{
				mSelectedItem = i;
				return true;
			}
		}
	return false;
}

void MenuGame::Loop()
{
	// background
	Env->Screen.BlitFrom(0, 0, PageMainMenu);

	// valid menu
	if (mCurrMenu != nullptr && mNumItem > 0)
	{

		// draw menu
		RenderMenu(Env, Vec2(8, 36), mCurrMenu, mNumItem, mSelectedItem,
			mFirstFrames / 4);

		// saturate first frames
		if (mFirstFrames > 0)
			mFirstFrames--;

		// move selected item?
		if ((Env->GameKey[KEY_P1_UP] && !Env->WasGameKey[KEY_P1_UP])
			|| (Env->GameKey[KEY_P2_UP] && !Env->WasGameKey[KEY_P2_UP]))
		{
			MoveSelectedItem(-1);
		}

		if ((Env->GameKey[KEY_P1_DOWN] && !Env->WasGameKey[KEY_P1_DOWN])
			|| (Env->GameKey[KEY_P2_DOWN] && !Env->WasGameKey[KEY_P2_DOWN]))
		{
			MoveSelectedItem(+1);
		}

		// left = back?
		if ((Env->GameKey[KEY_P1_LEFT] && !Env->WasGameKey[KEY_P1_LEFT])
			|| (Env->GameKey[KEY_P2_LEFT] && !Env->WasGameKey[KEY_P2_LEFT]))
		{
			mChargeNextGame = new IntroGame(Env);
		}

		// right = enter?
		if ((Env->GameKey[KEY_P1_RIGHT] && !Env->WasGameKey[KEY_P1_RIGHT])
			|| (Env->GameKey[KEY_P2_RIGHT] && !Env->WasGameKey[KEY_P2_RIGHT]))
		{
			MenuItem* mi = &mCurrMenu[mSelectedItem];
			if (mi->Kind == MI_Switch)
			{
				if (mi->State == false)
				{
					// switch all brothers off
					for (int i = 0; i < mi->NumBrother; i++)
						mCurrMenu[mi->Brothers[i]].State = false;

					// switch this on
					mi->State = true;
				}
			}

			if (mi->Kind == MI_Button)
			{
				if (mLoadIndex == 0 && mSelectedItem == 6)
				{
					// start game!!
					mChargeNextGame = new PacManGame(Env);
				}
			}
		}
	}

	// keep sound playing
	if (!Env->SoundIsPlaying)
		Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_MenuMusic;

	// timeout?
	if (Env->GameKey[KEY_ANY])
		mLastFrameIndexWithKey = Env->FrameCounter;
	if (Env->FrameCounter - mLastFrameIndexWithKey > GAME_FrameRate * 30)
	{
		mChargeNextGame = new IntroGame(Env);
	}

	// charged game?
	if (mChargeNextGame != nullptr && !Env->GameKey[KEY_ANY])
	{
		// finally wait for no key pressed, anymore
		Env->SoundAllStop = true;
		NextGame = mChargeNextGame;
	}

}
