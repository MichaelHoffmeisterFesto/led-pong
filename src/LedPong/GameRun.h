#pragma once

#include <algorithm>
#include "VectorArith.h"
#include "GameConst.h"

// The movement of the ghosts depends on this mode.
enum GhostMode {
	// Ghost is supposed to head towards a target tile which is located in the "home corner" of the ghost.
	Scatter,
	// Ghost tries to move accordingly / anticipating the Pac Man movements.
	Chase,
	// Ghost tries to flee from Pac Man.
	Freightened,
	// Ghost is frightened but will return to normal, soon.
	SuperFreightened
};

// This class holds a (volatile) state of a single run of a
// level / game. It encompasses also the game options.
class GameRun
{
public:
	GameRun();

	inline void Animate()
	{
		// To indicate different texts in one portion of the screen
		FramesToTextSwitch--;
		if (FramesToTextSwitch <= 0)
		{
			FramesToTextSwitch = GAME_Switch_Text_Frames;
			TextSwitch = 1 - TextSwitch;
		}

		// Message
		if (MsgLifeTime == 0)
			strcpy(Message, "");
		MsgLifeTime = std::max(0, MsgLifeTime - 1);

		// Frightened
		if (FrightenedCounter > GAME_FrameRate)
		{
			// is normal frightened
			FrightenedCounter--;
			GhostsMode = GhostMode::Freightened;
		}
		else if (FrightenedCounter > 1)
		{
			// will soon return to normal
			FrightenedCounter--;
			GhostsMode = ((FrightenedCounter % 12) < 6) ? GhostMode::SuperFreightened : GhostMode::Freightened;
		}
		else if (FrightenedCounter == 1)
		{
			// set back
			FrightenedCounter = 0;			
			GhostsMode = GhostMode::Chase;
		}

		// Count down(s)
		if (CountDownToChase > 0)
			CountDownToChase--;
		if (CountDownToScatter > 0)
			CountDownToScatter--;
	}

	inline void SetMessage(const char* msg)
	{
		strcpy(Message, msg);
		MsgLifeTime = GAME_Message_Life_time_frames;
	}

public:
	// How many frames in this run?
	int FrameCounter = 0;

	// Index of the current tile map 
	int TileMapIndex = 0;

	// Number of Level (how many levels pac man was going thru)
	int LevelNo = 1;

	// If greater zero, will cause a special display for a dead pacman and ghost going home
	int CountdownPacOrGhostDead = 0;

	// Phase [0.0 .. 1.0] for a special animation
	double SpecialAnimPhase = 0.0;

	// Step value to be added for every frame to the special animation phase
	double SpecialAnimStep = 0.0;

	// -1 means restart level, 0..3 means reset only 1 ghost position
	int SpecialResetOnlyGhost = -1;

	// For special animation of ghosts ..
	Vec2 SpecialAnimGhostDelta[GAME_Max_Ghost] = { 0 };

	// When initialized, remaining number (not points!) of pills in this level.
	// Note: If zero, subject to change to new level.
	int PillsAvailable = 0;

	// Mode of the ghosts. 
	GhostMode GhostsMode = GhostMode::Scatter;

	// Count down to switch from Scatter to Chase or vice versa
	int CountDownToChase = 0;
	int CountDownToScatter = 0;

	// If greater than zero, represents the time in frames where the ghosts are frightened.
	int FrightenedCounter = 0;

	// Some text ar divided into halfs. TextSwitch is either 0 or 1.
	int FramesToTextSwitch = 1;
	int TextSwitch = 1;

	// Indicate a short message
	char Message[GAME_Message_Max_Char + 1] = "";
	int MsgLifeTime = 0;

	// Number of players (either 1 or 2)
	int NumPlayer = 1;

	// Number of ghosts (typically 4)
	int NumGhost = 4;
};
