#pragma once

#include <algorithm>
#include "GameConst.h"

// This class holds a (volatile) state of a single run of a
// level / game.
class GameRun
{
public:
	GameRun();

	inline void Animate()
	{
		// Thought to be a (global) frame counter
		FrameCounter++;

		// To indicate different texts in one portion of the screen
		FramesToTextSwitch--;
		if (FramesToTextSwitch <= 0)
		{
			FramesToTextSwitch = GAME_Switch_Text_Frames;
			TextSwitch = 1 - TextSwitch;
		}

		// Message
		if (MsgLifeTime == 0)
			strcpy_s(Message, "");
		MsgLifeTime = std::max(0, MsgLifeTime - 1);
	}

	inline void SetMessage(const char* msg)
	{
		strcpy_s(Message, msg);
		MsgLifeTime = GAME_Message_Life_time_frames;
	}

public:
	// How many frames in this run?
	int FrameCounter = 0;

	// When initialized, remaining number (not points!) of pills in this level.
	// Note: If zero, subject to change to new level.
	int PillsAvailable = 0;

	// Some text ar divided into halfs. TextSwitch is either 0 or 1.
	int FramesToTextSwitch = 1;
	int TextSwitch = 1;

	// Indicate a short message
	char Message[GAME_Message_Max_Char + 1] = "";
	int MsgLifeTime = 0;

	// Number of players (either 1 or 2)
	int NumPlayer = 1;
};
