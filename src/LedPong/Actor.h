#pragma once

#include "VectorArith.h"
#include "GameRun.h"

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
		numPos = 4;
		Vec2* res = new Vec2[4];
		// up, left, down, right .. as described in the docu
		res[0] = Vec2(0, -1);
		res[1] = Vec2(-1, 0);
		res[2] = Vec2(0, +1);
		res[3] = Vec2(+1, 0);
		return res;
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

	// Care for (free-running) animation.
	inline virtual void Animate() { }

	// Take an animation step forward.
	inline virtual void MakeStep()
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

	// Set open mouth of player for a time
	void TriggerOpenMouth() { OpenMouthTime = 6; }

	// Get the character, which is to be rendered as avatar of the actor.
	char GetPlayerAvatarChar(GameRun& run, Vec2 direction);

	// take an animation step forward
	inline virtual void Animate()
	{
		Actor::Animate();
		if (OpenMouthTime > 0)
		{
			OpenMouthTime--;
		}
	}

public:
	// Attributes

	// Count down frames for an open mouth
	int OpenMouthTime = 0;

	// Total achieved score in points, basically energy pills + special items + won levels
	int Score = 0;

	// Number of remaining lives for this player
	int Lives = 3;
};

// The movement of the ghosts depends on this mode.
enum GhostMode { 
	// Ghost is supposed to head towards a target tile which is located in the "home corner" of the ghost.
	Scatter,		
	// Ghost tries to move accordingly / anticipating the Pac Man movements.
	Chase,
	// Ghost tries to flee from Pac Man.
	Freightened 
};

// Ghost class. Underlying foundation for all 4 individual ghosts.
class Ghost : public Actor
{
public:
	Ghost() {};

	Ghost(Ghost& other)
	{
	};

	virtual inline Actor* Clone() { return new Ghost(*this); }

	// Mode of the ghost. Externally synchronized.
	GhostMode Mode;

	// Tile (typically unreachable) representing the "home zone" of the ghost.
	// This leads typically to an orbiting behaviour near to the home zone while scattering.
	Vec2 HomeZone;

	// Get the character, which is to be rendered as avatar of the actor.
	inline virtual char GetGhostAvatarChar(GameRun& run, Vec2 direction)
	{
		return 'A';
	}

	inline virtual void Behaviour()
	{
		
	}
};