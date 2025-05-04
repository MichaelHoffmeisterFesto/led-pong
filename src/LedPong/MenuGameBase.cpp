#include <algorithm> // max, min
#include "basic.h"
#include "LedAnimation.h"
#include "LedColors.h"
#include "CQueueDynGeneric.h"

#include "GameConst.h"
#include "GameBase.h"
#include "MenuGameBase.h"

void MenuGameBase::LoadMenu(MenuItem* menu, int numItem, int startItem, string backgroundFn, Vec2 menuBasePos)
{
	// delete old menu structure?
	if (mCurrMenu != nullptr)
	{
		delete mCurrMenu;
		mCurrMenu = nullptr;
	}
	mNumItem = 0;

	// invalid
	if (numItem <= 0)
		return;

	// load new
	int len = sizeof(MenuItem) * numItem;
	mCurrMenu = (MenuItem*) malloc(len);
	if (mCurrMenu != nullptr)
		memcpy(mCurrMenu, menu, len);
	mNumItem = numItem;
	mSelectedItem = startItem;

	// load background
	if (backgroundFn != "")
		mPageBackground = LedTexture(backgroundFn);
	else
		mPageBackground = LedTexture();

	// base position (pixel fine)
	mMenuBasePos = menuBasePos;
}

MenuGameBase::MenuGameBase(GameEnvironment* env) : GameBase(env)
{
}

void RenderMenu(GameEnvironment* env, Vec2 startPos, MenuItem* menu, int numItem, int selectedItem, int extraRenderSpacingX)
{
	char buffer[80];

	for (int i = 0; i < numItem; i++)
	{
		// eval state
		MenuItem* mi = &menu[i];
		strcpy(buffer, "");
		switch (mi->Kind)
		{
			case MI_Switch:
				sprintf(buffer, "%c %s", mi->State ? mi->OnChar : mi->OffChar, mi->Text.c_str());
				break;

			default:
				strcpy(buffer, mi->Text.c_str());
				break;
		}

		// draw text
		Vec2 pos = startPos + Vec2(mi->X * 6, mi->Y * 9);
		env->TrPt.DrawTextTo(env->Screen, pos, buffer, 1 + extraRenderSpacingX, 1, 128,
			mi->NonPropText ? 5 : -1);

		// selected?
		if (i == selectedItem)
		{
			env->Screen.DrawRect(LedColors.White, pos.X - 2, pos.Y - 2, 2 + 6 * (0 + strnlen(buffer, sizeof(buffer))), 8);
		}
	}
}

MenuGameBase::~MenuGameBase()
{
	// delete old menu structure?
	if (mCurrMenu != nullptr)
	{
		// delete mCurrMenu;
		mCurrMenu = nullptr;
	}
}


bool MenuGameBase::MoveSelectedItem(int dir)
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

// overload this in derived classes!
GameBase* MenuGameBase::ButtonSelectLeft(int selectedItem)
{
	return nullptr;
}

// overload this in derived classes!
GameBase* MenuGameBase::ButtonSelectRight(int selectedItem)
{
	return nullptr;
}

void MenuGameBase::Loop()
{
	// background
	Env->Screen.BlitFrom(0, 0, mPageBackground);

	// valid menu
	if (mCurrMenu != nullptr && mNumItem > 0)
	{

		// draw menu
		RenderMenu(Env, mMenuBasePos, mCurrMenu, mNumItem, mSelectedItem,
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
			mChargeNextGame = ButtonSelectLeft(mSelectedItem);
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
				mChargeNextGame = ButtonSelectRight(mSelectedItem);
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
		mChargeNextGame = ButtonSelectLeft(mSelectedItem);
	}

	// charged game?
	if (mChargeNextGame != nullptr && !Env->GameKey[KEY_ANY])
	{
		// finally wait for no key pressed, anymore
		Env->SoundAllStop = true;
		NextGame = mChargeNextGame;
	}

}
