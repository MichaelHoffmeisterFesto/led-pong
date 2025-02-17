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

#include "CQueueDynGeneric.h"
#include "SpawnedItem.h"

#include "LedColor.h"
#include "LedColors.h"
#include "LedTexture.h"

#include "TextRendererFixedSize.h"
#include "TextRendererProportionalText.h"

#include "Tiles.h"
#include "TileMap.h"

#include "GameRun.h"
#include "GameConst.h"
#include "GameEnvironment.h"
#include "GameBase.h"

// This class is root class for the pac man game application itself.
// It holds the overall status and all important ressources
class PacManGame : public GameBase
{
public:
	PacManGame(GameEnvironment* env);
	~PacManGame();

	// Restarts ghost from initial pos, all modes reset, but level (pills, score, ..) untouched
	void RestartLevel();

	// Load a new tilemap into the level, restart everything
	void LoadLevel(int tileMapIndex, int levelNo);

	// Repeatedly called by the main application
	virtual void Loop();

public:
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

	// Several fruits are spawned concurrently and are removed one after each other again.
	CQueueDynGeneric<SpawnedItem> SpawnedItems;

	// state
	GameRun Run;
};

