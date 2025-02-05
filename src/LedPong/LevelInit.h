#pragma once

#include "basic.h"
#include "GameConst.h"
#include "TileMap.h"

// This class initializes the preset level array.
// It is outsourced for reasons for clarity & conciseness of the Game class.
class LevelInit
{
public:
	static void InitPresetLevels(int& levelNum, TileMap* levels[LEVEL_Max]);
};

