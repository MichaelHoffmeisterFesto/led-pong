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

#include "VectorArith.h"

#include "SpawnedItem.h"

#include "LedColor.h"
#include "LedColors.h"
#include "LedTexture.h"

#include "TextRendererFixedSize.h"
#include "TextRendererProportionalText.h"

#include "LedAnimation.h"

#include "CQueueDynGeneric.h"

#include "Tiles.h"
#include "TileMap.h"

#include "GameRun.h"
#include "GameConst.h"

// enum for key input
enum GameKeyEnum { 
	KEY_P1_LEFT, KEY_P1_RIGHT, KEY_P1_UP, KEY_P1_DOWN, 
	KEY_P2_LEFT, KEY_P2_RIGHT, KEY_P2_UP, KEY_P2_DOWN, 
	KEY_DEBUG,
	KEY_MAX_NUM 
};

enum GameSoundSampleEnum {
	SMP_None, SMP_EmptyTile, SMP_EnergyPill, SMP_TurnToGhosts, SMP_TurnFromGhosts, 
	SMP_Fruit, SMP_PacManDead, SMP_GhostDead, SMP_LevelWin, SMP_MAX_NUM
};

// This class is the master / root class for the game application.
// It holds the overall status and all important ressources
class Game
{
public:
	Game();
	~Game();

	void AddWelcomeAnimations();

	// Restarts ghost from initial pos, all modes reset, but level (pills, score, ..) untouched
	void RestartLevel();

	// Load a new tilemap into the level, restart everything
	void LoadLevel(int tileMapIndex, int levelNo);

	void Loop();

public:
	// actual input keys .. set by the main function
	bool GameKey[KEY_MAX_NUM] = { false } ;

	// old / delayed by one cycle .. values of the keys
	bool WasGameKey[KEY_MAX_NUM] = { false };

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

	// ghosts
	Blinky blinky;
	Pinky pinky;
	Inky inky;
	Clyde clyde;
	Ghost* Ghosts[4] = { &blinky, &pinky, &inky, &clyde };

	// Sound sample played by main application at end of the loop cycle.
	GameSoundSampleEnum SoundSampleToPlay = GameSoundSampleEnum::SMP_None;

	// monitored items

	// Several fruits are spawned concurrently and are removed one after each other again.
	CQueueDynGeneric<SpawnedItem> SpawnedItems;

	// state
	GameRun Run;
	int openMouthTime = 0;
	bool showDebug = false;
};

