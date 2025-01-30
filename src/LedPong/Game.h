#pragma once

using namespace std;

#include <algorithm> // max, min

#include <iostream>

#include <stdint.h> // for standard integer types like uint8_t ..

#include <cstdlib> // for srand() and rand()
#include <ctime>   // for time()

#include <SDL.h>
#include <SDL_ttf.h>
#include "SDL_Helper.h"
#include "SDL_Application.h"

#include "basic.h"

#include "LedColor.h"
#include "LedColors.h"
#include "LedTexture.h"

#include "TextRendererFixedSize.h"
#include "TextRendererProportionalText.h"

#include "LedAnimation.h"

#include "CQueueDynGeneric.h"

#include "SampleGen.h"

#include "Tiles.h"
#include "TileMap.h"

// GAME

const int GAME_FrameRate = 30;

// dimensions

// 96:128 = 3:4
const int WALL_Xdim = 3 * 32;
const int WALL_Ydim = 2 * 64;

const int WALL_Xgap = 1;
const int WALL_Ygap = 1;

const int WALL_XpixSize = 4;
const int WALL_YpixSize = 4;

const int LEVEL_Max = 10;

// enum for key input
enum GameKeyEnum { 
	KEY_P1_LEFT, KEY_P1_RIGHT, KEY_P1_UP, KEY_P1_DOWN, 
	KEY_P2_LEFT, KEY_P2_RIGHT, KEY_P2_UP, KEY_P2_DOWN, 
	KEY_MAX_NUM 
};

enum GameSoundSampleEnum {
	None, EnergyPill, SOUND_SAMPLE_MAX_NUM
};

// This class is the master / root class for the game application.
// It holds the overall status and all important ressources
class Game
{
public:
	Game();
	~Game();

	void AddWelcomeAnimations();

	void LoadLevel(TileMap* level);

	void Loop();

public:
	// input keys
	bool GameKey[KEY_MAX_NUM] = { false } ;

	// ressources
	LedTexture PageBallons;
	LedTexture PageFunnyCar;
	LedTexture PageInfo2;

	// components
	CQueueDynGeneric<LedAnimation> AnimationQueue;

	// renderers
	TextRendererFixedSize TrFs;
	TextRendererProportionalText TrPt;

	// main LED screen
	LedTexture Screen;

	// levels
	TileMap* Levels[LEVEL_Max] = { 0 };
	int LevelNum = 0;
	TileMap* LevelCurr;

	// actors
	Player Player1, Player2;
	Player* Players[2] = { &Player1, &Player2 };

	// Sound sample played by main application at end of the loop cycle.
	GameSoundSampleEnum SoundSampleToPlay = GameSoundSampleEnum::None;

	// state
	int FrameCounter = 0;
	int openMouthTime = 0;

};

