#pragma once

#include "VectorArith.h"

// This class keeps track on a spawned item on the tile map
class SpawnedItem
{
public:

	SpawnedItem();
	SpawnedItem(Vec2 position, int frameCountdown);

	// Tile position of the spawned item
	Vec2 Position;

	// Count down evry frame until zero to delete spawned item again
	int FrameCountdown = 0;
};