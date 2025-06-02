#include "basic.h"

#include "Actor.h"
#include "TileMap.h"

#include <iostream>
#include <algorithm>

using namespace std;

#include "Tiles.h"
#include "TileFactory.h"
#include "LedTexture.h"
#include "TextRendererAbstractBase.h"
#include "GameRun.h"

// Empty constructor
TileMap::TileMap()
{
	mWidth = 0;
	mHeight = 0;
	mTiles = nullptr;
}

// Constructor for definining an empty tile map by explicit dimensions
TileMap::TileMap(int width, int height)
{
	mWidth = width;
	mHeight = height;
	mTiles = new TileBase *[mWidth * mHeight];
	TileBase* tile = TileFactory::GenerateTile(' ');
	Fill(tile);
}

// Constructor for taking over an existing source of pixel
TileMap::TileMap(int width, int height, TileBase** source, bool doCopy)
{
	mWidth = width;
	mHeight = height;

	if (!doCopy)
		mTiles = source;
	else
	{
		int num = mWidth * mHeight;
		mTiles = new TileBase*[num];
		for (int i = 0; i < num; i++)
			mTiles[i] = source[i]->Clone();
	}
}

TileMap::TileMap(int numRow, string rows[])
{
	// determine max row, col
	int maxRow = 0, maxCol = 0;
	for (int i = 0; i < numRow; i++)
	{
		maxRow = std::max(maxRow, 1 + i);
		maxCol = std::max(maxCol, (int) rows[i].length());
	}

	// set this
	mHeight = maxRow;
	mWidth = maxCol;
	int num = mWidth * mHeight;
	if (num < 1)
		return;
	
	// allocate and fill with empty
	mTiles = new TileBase*[num];
	{
		TileBase* tile = TileFactory::GenerateTile(' ');
		Fill(tile);
	}

	// go thru rows again
	for (int ri = 0; ri < numRow; ri++)
		for (int ci = 0; ci < (int) rows[ri].length(); ci++)
		{
			TileBase* tile = TileFactory::GenerateTile(rows[ri].at(ci));
			Put(ci, ri, tile);
		}

	// updates
	UpdatePillsAvailable();
}

// Will count and update the current number (not points!) of energy pills in the map.
void TileMap::UpdatePillsAvailable()
{
	int count = 0;
	for (int y = 0; y < mHeight; y++)
		for (int x = 0; x < mWidth; x++)
		{
			Tile* tile = GetAsTile(x, y);
			if (tile != nullptr && tile->IsEnergyPill())
				count++;
		}
	PillsTotal = count;
}

// destructor
TileMap::~TileMap()
{
	if (mTiles != nullptr)
	{
		int num = mWidth * mHeight;
		for (int i = 0; i < num;i++)
			if (mTiles[i] != nullptr)
				delete mTiles[i];
		delete[] mTiles;
        mTiles = nullptr;
	}
}

// clone
TileMap* TileMap::Clone()
{
	TileMap* res = new TileMap(mWidth, mHeight, mTiles, true);

	// very hacky
#define __FULL_COPY(X) copy_n(X, SIZE_OF_ARR(X), res->X);
	__FULL_COPY(PlayerStartPos);
	__FULL_COPY(GhostStartPos);
	__FULL_COPY(GhostHomeZone);
	res->GateToLeft = GateToLeft;
	res->GateToTop = GateToTop;
	res->GateToRight = GateToRight;
	res->GateToBottom = GateToBottom;
	__FULL_COPY(PlayerTextScorePos);
	__FULL_COPY(PlayerTextExtraPos);
	res->MessagePos = MessagePos;
	res->PillsTotal = PillsTotal;
#undef __FULL_COPY

	return res;
}

// save
TileBase* TileMap::Get(int x, int y)
{
	if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
		return nullptr;
	return mTiles[mWidth * y + x];
}

Tile* TileMap::GetAsTile(int x, int y)
{
	TileBase* tile = Get(x, y);
	if (instanceof<Tile>(tile))
		return dynamic_cast<Tile*>(tile);
	return nullptr;
}

// save
void TileMap::Put(int x, int y, TileBase* preset)
{
	if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
		return;
	mTiles[mWidth * y + x] = preset;
}

void TileMap::Fill(TileBase* preset)
{
	for (int y = 0; y < mHeight; y++)
		for (int x = 0; x < mWidth; x++)
		{
			mTiles[mWidth * y + x] = preset->Clone();
		}
}

// draw the map
bool TileMap::DrawMap(GameRun& run, LedTexture& texture, Vec2 pixelPos,
	TextRendererAbstractBase& textRenderer,
	int charWidth, int charHeight)
{
	// access
	if (!texture.HasContent())
	{
		return false;
	}

	// work coordinates
	Vec2 workPos = pixelPos;

	// build individual rows
	for (int ri = 0; ri < mHeight; ri++)
	{
		// generate a string
		string row = "";
		for (int ci = 0; ci < mWidth; ci++)
		{
			TileBase* tile = Get(ci, ri);
			if (instanceof<Tile>(tile))
			{
				row += dynamic_cast<Tile*>(tile)->GetTileAvatarChar(run);
			}
			else
				row += ' ';
		}

		// display this string
		textRenderer.DrawTextTo(texture, workPos, row.c_str());
		workPos.Y += charHeight;
	}

	return true;
}

PossibleMove* TileMap::GetPossibleMovesFor(Actor* actor, Vec2 checkPos, int& numMoves)
{
	// access
	numMoves = 0;
	if (!CheckPos(checkPos))
	{
		return nullptr;
	}

	// acquire every basic direction
	int numDir = 0;
	Vec2* basicDirs = actor->GetBasicDirections(numDir);
	
	// possible moves are not more than basic directions
	PossibleMove* moves = new PossibleMove[numDir];

	// Note: This is not as bad / cumbersome, as it looks
	// Note: Accept the performance loss because its elegant.
	Vec2 tmpDirs[] = { Vec2(-1,0), Vec2(+1,0), Vec2(0,-1), Vec2(0,+1) };
	Vec2 tmpGates[] = { GateToLeft, GateToRight, GateToTop, GateToBottom };
	
	// try out
	for (int di = 0; di < numDir; di++)
	{
		// before even checking for moves, could we do a jump because of a gate position?
		bool jumpTaken = false;
		for (int gi = 0; gi < (int) (int) (int) (int) (int) (int) (int) (int) SIZE_OF_ARR(tmpGates); gi++)
		{
			if (!tmpGates[gi].IsValid || !(checkPos == tmpGates[gi]) || !(basicDirs[di] == tmpDirs[gi]))
				continue;

			// as it is valid, think out an "jump to" final position
			int npi = 0;
			do {
				npi = rand() % SIZE_OF_ARR(tmpGates);
			} while (npi == gi || !tmpGates[npi].IsValid);

			// take as final pos
			moves[numMoves++] = PossibleMove(checkPos, basicDirs[di], tmpGates[npi], true);

			// assume, that the "normal" move is not taken ..
			jumpTaken = true;
		}
		
		if (!jumpTaken)
		{
			// what would be the final pos?
			Vec2 finalPos = checkPos + basicDirs[di];

			// final pos needs to be valid
			if (!CheckPos(finalPos))
				continue;

			// needs to end on an moveable tile
			Tile* tp = GetAsTile(finalPos.X, finalPos.Y);
			if (tp == nullptr || !tp->IsMoveablePosition())
				continue;

			// already a winner!
			moves[numMoves++] = PossibleMove(checkPos, basicDirs[di], finalPos, false);
		}
	}

	// have results
	return moves;
}

bool TileMap::FindPossibleMoveInDir(Actor* actor, Vec2 checkPos, Vec2 dir, PossibleMove& move)
{
	bool res = false;
	int numMoves = 0;
	PossibleMove* moves = GetPossibleMovesFor(actor, checkPos, numMoves);
	for (int i = 0; i < numMoves; i++)
	{
		if (dir == moves[i].Direction)
		{
			move = moves[i];
			res = true;
		}
	}
	delete moves;
	return res;
}

int TileMap::CountTileCode(char code)
{
	int res = 0;for (int y = 0; y < mHeight; y++)
		for (int x = 0; x < mWidth; x++)
		{
			// find
			Tile* tile = GetAsTile(x, y);
			if (tile != nullptr && tile->mTileCode == code)
				res++;
		}
	return res;
}

Vec2 TileMap::FindRandomFreeEnergyPillPosition()
{
	// this is a job done in two passes over the free energy pills
	int choosenIndex = 0;
	for (int pass = 0; pass < 2; pass++)
	{
		int numIndex = 0;
		for (int y = 0; y < mHeight; y++)
			for (int x = 0; x < mWidth; x++)
			{
				// find
				Tile* tile = GetAsTile(x, y);
				if (tile == nullptr || !tile->IsEnergyPill())
					continue;

				// choose an index?
				if (pass == 1 && numIndex == choosenIndex)
					return Vec2(x, y);

				// in any case, count the index up
				numIndex++;
			}

		// after 1st pass, choose an index
		// if no index, return with invalid
		if (numIndex < 1)
			return Vec2();
		choosenIndex = rand() % numIndex;
	}

	// uups
	return Vec2();
}
