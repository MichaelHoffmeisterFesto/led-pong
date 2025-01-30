#pragma once

#include <iostream>

#include "LedTexture.h"
#include "TextRendererAbstractBase.h"
#include "Tiles.h"
#include "VectorArith.h"
#include "Actor.h"

using namespace std;

class PossibleMove
{
public:
	// This reflects the actual zero-based (0,0) position of an actor.
	Vec2 ActualPos;

	// This is a zero-based (0,0) directional vector, typical |(x,y)| <= 1.41.
	Vec2 Direction;

	// This indicates the final position of the actor AFTER the move.
	// Note: This could do a "jump" from one gate to another.
	Vec2 FinalPosition;

	// True, if it is a "jump" movement from one gate to another.
	bool JumpMove = false;

	// provide some "rich" constructor
	PossibleMove() {};

	PossibleMove(Vec2 actualPos, Vec2 direction, Vec2 finalPos, bool jumpMove)
		: ActualPos(actualPos), Direction(direction), FinalPosition(finalPos), JumpMove(jumpMove)
	{};
};

// This class realizes a two-dimensional map of single tiles.
class TileMap
{
public:
	// constructors
	TileMap();
	TileMap(int width, int height);
	TileMap(int width, int height, TileBase** source, bool doCopy);
	TileMap(int numRow, string rows[]);

	// destructor
	~TileMap();

	// cloning
	TileMap& Clone();

	// accessors
	inline int GetWidth() { return mWidth; }
	inline int GetHeight() { return mHeight; }

	inline bool CheckXY(int x, int y)
	{
		return x >= 0 && x < mWidth && y >= 0 && y < mHeight;
	}

	inline bool CheckPos(Vec2 pos)
	{
		return pos.IsValid && pos.X >= 0 && pos.X < mWidth && pos.Y >= 0 && pos.Y < mHeight;
	}

	inline bool HasContent()
	{
		return mTiles != nullptr && mWidth >= 0 && mHeight >= 0;
	}

	// safe, access of single tiles
	TileBase* Get(int x, int y);
	Tile* GetAsTile(int x, int y);
	inline Tile* GetAsTile(Vec2 pos) { return GetAsTile(pos.X, pos.Y); }
	void Put(int x, int y, TileBase* preset);

	// fill all tiles in the map
	void Fill(TileBase* preset);

	// draw the map
	bool DrawMap(LedTexture& texture, Vec2 pixelPos,
		TextRendererAbstractBase &textRenderer,
		int charWidth, int charHeight);

	// Determine all possible moves for an actor for a given position.
	// Note: will return a dynamic array of moves with "numMoves", which has to be freed
	// even if "numMoves" is zero!
	PossibleMove* GetPossibleMovesFor(Actor* actor, Vec2 checkPos, int& numMoves);

	// Checks if a move in a given direction is possible.
	bool FindPossibleMoveInDir(Actor* actor, Vec2 checkPos, Vec2 dir, PossibleMove& move);

public:
	// some public attributes to the map (without getter/ setter/ constructor).
	Vec2 PlayerStartPos[2];
	Vec2 GhostStartPos[4];
	Vec2 GateToLeft, GateToTop, GateToRight, GateToBottom;

	// Total number of pills on the map
	int PillsTotal = 0;
	
	// When initialized, remaining number (not points!) of pills.
	// Note: If zero, subject to change to new level.
	int PillsAvailable = 0;

private:
	int mWidth = 0;
	int mHeight = 0;
	TileBase** mTiles = nullptr;

	// Will count and update the current number (not points!) of energy pills in the map.
	void UpdatePillsAvailable();
};

