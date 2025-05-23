#pragma once

#include <iostream>

#include "LedTexture.h"
#include "TextRendererAbstractBase.h"
#include "Tiles.h"
#include "VectorArith.h"
#include "Actor.h"
#include "GameRun.h"

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
	TileMap* Clone();

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
	inline Tile* GetAsTile(Vec2 pos) { return (pos.IsValid) ? GetAsTile(pos.X, pos.Y) : nullptr; }
	void Put(int x, int y, TileBase* preset);

	// fill all tiles in the map
	void Fill(TileBase* preset);

	// draw the map
	bool DrawMap(GameRun& run, LedTexture& texture, Vec2 pixelPos,
		TextRendererAbstractBase &textRenderer,
		int charWidth, int charHeight);

	// Determine all possible moves for an actor for a given position.
	// Note: will return a dynamic array of moves with "numMoves", which has to be freed
	// even if "numMoves" is zero!
	PossibleMove* GetPossibleMovesFor(Actor* actor, Vec2 checkPos, int& numMoves);

	// Checks if a move in a given direction is possible.
	bool FindPossibleMoveInDir(Actor* actor, Vec2 checkPos, Vec2 dir, PossibleMove& move);

	// For a given set of basic directions and a position on the map, count the adjacent tiles, which are a moveable position.
	inline int CountMoveableDirections(Vec2 pos, int numPos, Vec2* dirs)
	{
		int res = 0;
		for (int i = 0; i < numPos; i++)
		{
			Tile* tp = GetAsTile(pos + dirs[i]);
			if (tp != nullptr && tp->IsMoveablePosition())
				res++;
		}
		return res;
	}

	// Counts all tiles which are of a special code
	int CountTileCode(char code);

	// Find a position, which is occupated by an energy pill and nothing else.
	Vec2 FindRandomFreeEnergyPillPosition();

public:
	// some public attributes to the map (without getter/ setter/ constructor).
	Vec2 PlayerStartPos[2];
	Vec2 GhostStartPos[4];
	Vec2 GhostHomeZone[4];
	Vec2 GateToLeft, GateToTop, GateToRight, GateToBottom;
	Vec2 PlayerTextScorePos[2];
	Vec2 PlayerTextExtraPos[2];
	Vec2 MessagePos;
	
	void UpdatePillsAvailable();

	// Total number of pills on the map
	int PillsTotal = 0;
	
private:
	int mWidth = 0;
	int mHeight = 0;
	TileBase** mTiles = nullptr;

	// Will count and update the current number (not points!) of energy pills in the map.
};

