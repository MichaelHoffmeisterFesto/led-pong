#pragma once

#include <algorithm> // max, min
#include "basic.h"
#include "LedAnimation.h"
#include "CQueueDynGeneric.h"

#include "GameConst.h"
#include "GameEnvironment.h"
#include "GameBase.h"

// This class plays (autonomously) an introduction, until a key is pressed
class IntroGame : public GameBase
{
public:
	IntroGame(GameEnvironment* env);
	~IntroGame();

	// Repeatedly called by the main application
	virtual void Loop();

private:
	// ressources
	LedTexture PageEyedBall;
	LedTexture PageManyPacs;
	LedTexture PageInfo;
	LedTexture PageTechMan;
	LedTexture PageRainbow;
	LedTexture PageCredits;

	// Re-add the animations to the queue
	void AddWelcomeAnimations();
	
	// Queue of animations
	CQueueDynGeneric<LedAnimation> AnimationQueue;
};

