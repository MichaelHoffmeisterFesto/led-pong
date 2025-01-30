#pragma once

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

	virtual inline bool IsMoveablePosition() { return mTileCode == '.' || mTileCode == ' '; }
	
	char mTileCode = ' ';
};
