#pragma once

#include "VectorArith.h"
#include "GameConst.h"
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

	// The actor is actively waiting.
	inline bool IsWaiting() { return WaitAtCurrentPos > 0; }

	// Actor is without movement. When not StandStill(), Animate() shall be called
	// until the actor has reached StandStill() again.
	inline bool StandStill()
	{
		if (Phase > 1.0)
			Phase = 0;
		return Phase == 0;
	}

	// Calculate, based on Phase, the current pixel position
	inline Vec2 GetCurrentPixelPos(Vec2 charDim)
	{
		return CurrentTilePosition * charDim + CurrentDirection * (int) (1.0 * charDim.X * Phase);
	}

	// Care for (free-running) animation.
	inline virtual void Animate(GameRun& run) { 
		if (WaitAtCurrentPos > 0)
			WaitAtCurrentPos--;
	}

	// Take an animation step forward.
	inline virtual void MakeStep(GameRun& run)
	{
		Phase += (run.GhostsMode == GhostMode::Freightened
			      || run.GhostsMode == GhostMode::SuperFreightened) ? PhaseStepFright : PhaseStepNorm;
		
		if (Phase > 1.0)
		{
			Phase = 0;
			CurrentTilePosition = CurrentTilePosition + CurrentDirection;
		}
	}

public:
	// Current position on the tile map
	Vec2 CurrentTilePosition;

	// Current direction as unity vector
	Vec2 CurrentDirection;

	// Phase of animation; 0 = CurrentTilePosition; FinalPhase = last phase before stand still
	double Phase = 0;

	// Fractal step of complete movement (1.0) for normal movement
	double PhaseStepNorm = GAME_PacMan_Phase_per_Frame;

	// Fractal step of complete movement (1.0) for movement while ghosts are frightened
	double PhaseStepFright = GAME_PacMan_Phase_per_Frame;

	// If greater zero, will wait at current pos before proceeding ..
	int WaitAtCurrentPos = 0;

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

	// Which char to show for a dead player
	inline virtual char GetDeadPlayerAvatar(double phase) { return ((int)(10.0 * phase) % 2) ? ' ' : 'W'; }

	// take an animation step forward
	inline virtual void Animate(GameRun& run)
	{
		Actor::Animate(run);
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

// Ghost class. Underlying foundation for all 4 individual ghosts.
class Ghost : public Actor
{
public:
	inline virtual void ResetToInitialPosition(Vec2 initialPos)
	{
		CurrentTilePosition = initialPos;
		Phase = 0.0;
		WaitAtCurrentPos = 4 * GAME_FrameRate;
	}

	// As the target positions of the ghost is the most influential difference between the ghosts, 
	// this is being realized by the different sub classes of Ghost.
	// see: https://web.archive.org/web/20241229103340/http://gameinternals.com/understanding-pac-man-ghost-behavior
	inline virtual Vec2 GetCurrentTargetPos(GameRun& run, Vec2 pacPos, Vec2 pacDir, Ghost** siblings) 
	{ 
		return HomeZone; 
	}

	// Just a helper to have the different chars for modes
	inline char PrepareAvatarChar(GameRun& run, char normalAvatar)
	{
		return
			(run.GhostsMode == GhostMode::SuperFreightened) ? '�'
			: ( (run.GhostsMode == GhostMode::Freightened) ? '$' : normalAvatar );
	}

	// Get the character, which is to be rendered as avatar of the actor.
	inline virtual char GetGhostAvatarChar(GameRun& run, Vec2 direction) = 0;

public:
	// Tile (typically unreachable) representing the "home zone" of the ghost.
	// This leads typically to an orbiting behaviour near to the home zone while scattering.
	Vec2 HomeZone;

	// for debugging purposes, this is held inside the class
	Vec2 UseTargetPosition;
};

// Individual ghost class
class Blinky : public Ghost
{
public:
	Blinky() {};

	Blinky(Blinky& other)
	{
	};

	virtual inline Actor* Clone() { return new Blinky(*this); }

	inline virtual Vec2 GetCurrentTargetPos(GameRun& run, Vec2 pacPos, Vec2 pacDir, Ghost** siblings)
	{
		// in chase, just go to pac
		return (run.GhostsMode == GhostMode::Scatter) ? HomeZone : pacPos ;
	}

	inline virtual char GetGhostAvatarChar(GameRun& run, Vec2 direction)
	{
		return PrepareAvatarChar(run, 'A');
	}
};

// Individual ghost class
class Pinky : public Ghost
{
public:
	Pinky() {};

	Pinky(Pinky& other)
	{
	};

	virtual inline Actor* Clone() { return new Pinky(*this); }

	inline virtual Vec2 GetCurrentTargetPos(GameRun& run, Vec2 pacPos, Vec2 pacDir, Ghost** siblings)
	{
		// in chase, go to pac's position and 4 times the direction further
		return (run.GhostsMode == GhostMode::Scatter) ? HomeZone : pacPos + pacDir * 4;
	}

	inline virtual char GetGhostAvatarChar(GameRun& run, Vec2 direction)
	{
		return PrepareAvatarChar(run, 'B');
	}
};

// Individual ghost class
class Inky : public Ghost
{
public:
	Inky() {};

	Inky(Inky& other)
	{
	};

	virtual inline Actor* Clone() { return new Inky(*this); }

	inline virtual Vec2 GetCurrentTargetPos(GameRun& run, Vec2 pacPos, Vec2 pacDir, Ghost** siblings)
	{
		// in chase, take the vector from Blinky to Pac times 2 from Blink and go there
		return (run.GhostsMode == GhostMode::Scatter) 
			? HomeZone 
			: (siblings[0]->CurrentTilePosition + ( ( pacPos + pacDir * 2 ) - siblings[0]->CurrentTilePosition ) * 2);
	}

	inline virtual char GetGhostAvatarChar(GameRun& run, Vec2 direction)
	{
		return PrepareAvatarChar(run, 'C');
	}
};

// Individual ghost class
class Clyde : public Ghost
{
public:
	Clyde() {};

	Clyde(Clyde& other)
	{
	};

	virtual inline Actor* Clone() { return new Clyde(*this); }

	inline virtual Vec2 GetCurrentTargetPos(GameRun& run, Vec2 pacPos, Vec2 pacDir, Ghost** siblings)
	{
		// in chase, go either to pac's position and when to close, restrain and go to home zone
		if (run.GhostsMode == GhostMode::Scatter)
			return HomeZone;
		double dist = (pacPos - CurrentTilePosition).Length();
		if (dist > 8.0)
			return pacPos;
		return HomeZone;
	}

	inline virtual char GetGhostAvatarChar(GameRun& run, Vec2 direction)
	{
		return PrepareAvatarChar(run, 'D');
	}
};