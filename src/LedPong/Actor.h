#pragma once

#include "VectorArith.h"

// This class defines the base class for an actor, that is, an moving
// object on a map, either controlled by a player (input) or by an
// ghost algorithmn.
class Actor
{
public:
	Actor()
	{
	};

	Actor(Actor& other)
	{
	};

	virtual inline Actor* Clone() { return new Actor(*this); }

	// Gets the theoretical directions an actor could move.
	// Note: The methods allocates an array, which needs to be freed.
	virtual inline Vec2* GetBasicDirections(int& numPos) {
		numPos = 0;
		return nullptr;
	}

	// Current position on the tile map
	Vec2 CurrentTilePosition;

	// Current direction as unity vector
	Vec2 CurrentDirection;

	// Phase of animation; 0 = CurrentTilePosition; FinalPhase = last phase before stand still
	int Phase = 0;

	// Last (final) phase value, before movement is over
	int FinalPhase = 1;

	// Actor is without movement. When not StandStill(), Animate() shall be called
	// until the actor has reached StandStill() again.
	inline bool StandStill()
	{
		if (Phase > FinalPhase)
			Phase = 0;
		return Phase == 0;
	}

	// Calculate, based on Phase, the current pixel position
	inline Vec2 GetCurrentPixelPos(Vec2 charDim)
	{
		return CurrentTilePosition * charDim + CurrentDirection * Phase;
	}

	// take an animation step forward
	inline void Animate()
	{
		Phase++;
		if (Phase > FinalPhase)
		{
			Phase = 0;
			CurrentTilePosition = CurrentTilePosition + CurrentDirection;
		}
	}
};

// Player class
class Player : public Actor
{
public:
	Player() {};

	Player(Player& other)
	{
	};

	virtual inline Actor* Clone() { return new Player(*this); }

	virtual inline Vec2* GetBasicDirections(int& numPos) {
		numPos = 4;
		Vec2* res = new Vec2[4];
		res[0] = Vec2(-1, 0);
		res[1] = Vec2(+1, 0);
		res[2] = Vec2(0, -1);
		res[3] = Vec2(0, +1);
		return res;
	}

	char GetPlayerPhase(Vec2 direction, bool openMouth);

public:
	// Attributes

	// Total achieved score in points, basically energy pills + bonus
	int Score = 0;
};