#pragma once

using namespace std;

#include <algorithm> // max, min

#include "basic.h"
#include "GameEnvironment.h"

// This class is the base class for all games.
// The idea is, that the application can consinst of multiple games.
// Also start screen, menues etc would be individual games derived from this game base class.
class GameBase
{
public:
	// There MUST an environment provided!
	GameBase(GameEnvironment* env) : Env(env) {}

	// Repeatedly called by the main application
	virtual void Loop() {}

public:
	// MUST be available to all games
	GameEnvironment* Env;
};

