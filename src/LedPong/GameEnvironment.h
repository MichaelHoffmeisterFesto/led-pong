#pragma once

#include "basic.h"

#include "LedTexture.h"

#include "TextRendererFixedSize.h"
#include "TextRendererProportionalText.h"

// enum for key input
enum GameKeyEnum {
	KEY_ANY,
	KEY_P1_LEFT, KEY_P1_RIGHT, KEY_P1_UP, KEY_P1_DOWN,
	KEY_P2_LEFT, KEY_P2_RIGHT, KEY_P2_UP, KEY_P2_DOWN,
	KEY_DEBUG1, KEY_DEBUG2, KEY_DEBUG3,
	KEY_MUTE, KEY_GOD_MODE,
	KEY_MAX_NUM
};

enum GameSoundSampleEnum {
	SMP_None, SMP_EmptyTile, SMP_EnergyPill, SMP_TurnToGhosts, SMP_TurnFromGhosts,
	SMP_Fruit, SMP_PacManDead, SMP_GhostDead, SMP_LevelWin, SMP_IntroMusic, SMP_MenuMusic,
	SMP_MAX_NUM
};

// This class implement variables, states, ressources shared by ALL game class instances
class GameEnvironment
{
public:
	GameEnvironment();
	~GameEnvironment();

	inline void Animate()
	{
		// Thought to be a (global) frame counter
		FrameCounter++;

		// some key events
		if (GameKey[KEY_MUTE] && !WasGameKey[KEY_MUTE])
			Mute = Mute;

		// allow slope detection of keys
		memcpy(WasGameKey, GameKey, sizeof(GameKey));
	}

public:
	// actual input keys .. set by the main function
	bool GameKey[KEY_MAX_NUM] = { false };

	// old / delayed by one cycle .. values of the keys
	bool WasGameKey[KEY_MAX_NUM] = { false };

	// main LED screen
	LedTexture Screen;

	// renderers
	TextRendererFixedSize TrFs;
	TextRendererProportionalText TrPt;

	// How many frames in this run?
	int FrameCounter = 0;

    // Sound enables
    bool SoundEnable = true;

	// Sound turned on or off
	bool Mute = false;

	// Keys swapped
	bool KeySwap = false;

	// immortal
	bool GodMode = false;

	// Allow debug and god mode?
	bool AllowDebug = false;

	// Allow debug and god mode?
	bool ShowDebug = false;

	// true, if any sound is playing
	bool SoundIsPlaying = false;

	// true, if to stop all sound
	bool SoundAllStop = false;

	// Sound sample played by main application at end of the loop cycle.
	GameSoundSampleEnum SoundSampleToPlay = GameSoundSampleEnum::SMP_None;
};

