#pragma once

#include "GameRun.h"

// Base class of all tile types.
// For future extensibility.
class TileBase
{
public:
	TileBase() 
	{
	};

	TileBase(TileBase& other)
	{
	};

	virtual inline TileBase* Clone() { return new TileBase(*this); }
};

// Normal tile identified by on character-based tile code.
class Tile : public TileBase
{
public:
	Tile() { };

	Tile(Tile& other) 
		: mTileCode(other.mTileCode)
	{ };

	virtual inline TileBase* Clone() { return new Tile(*this); }

	virtual inline bool IsEnergyPill() { return mTileCode == '.'; }

	virtual inline bool IsFruit() { return mTileCode == 'c' || mTileCode == 'e' || mTileCode == 'g'; }

	virtual inline bool IsEnergizer(GameRun& run) { 
		return mTileCode == '*' && (run.FrameCounter % 300) < 90; 
	}
	virtual inline bool IsEmptyPill() { return mTileCode == ' '; }

	virtual inline bool IsMoveablePosition() { 
		return mTileCode == '.' || mTileCode == ' ' || mTileCode == '*'
			|| mTileCode == 'c' || mTileCode == 'e' || mTileCode == 'g';
	}

	virtual inline bool IsEatablePosition() {
		return mTileCode == '.' || mTileCode == '*'
			|| mTileCode == 'c' || mTileCode == 'e' || mTileCode == 'g';
	}

	virtual inline char GetTileAvatarChar(GameRun& run) { 
		if (mTileCode == '*')
			return IsEnergizer(run) ? '*' : '.';
		return mTileCode; 
	}
	
	// This denotes the underlying code of the tile. It could be rendered on different ways.
	char mTileCode = ' ';
};
