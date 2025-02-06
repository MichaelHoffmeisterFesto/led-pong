#include <algorithm> // max, min
#include "basic.h"
#include "LedAnimation.h"
#include "CQueueDynGeneric.h"

#include "GameBase.h"
#include "IntroGame.h"

IntroGame::IntroGame(GameEnvironment* env) : GameBase(env)
{
	// Ressources
	PageBallons = LedTexture("media/ballons.bmp");
	PageFunnyCar = LedTexture("media/ai_funny_car.bmp");
	PageInfo2 = LedTexture("media/HKA_Info2.bmp");

	// first sequence of animations
	AddWelcomeAnimations();
}

IntroGame::~IntroGame()
{
}

void IntroGame::AddWelcomeAnimations()
{
	AnimationQueue.enqueue(LedAnimation(
		PageInfo2, 1.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect()));

	AnimationQueue.enqueue(LedAnimation(
		PageFunnyCar, 1.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect()));

	AnimationQueue.enqueue(LedAnimation(
		PageBallons, 2.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect()));
}

void IntroGame::Loop()
{
	double step = 0.01;

	auto ta = AnimationQueue.top();
	LedTexture blend = Env->Screen;
	if (ta != nullptr)
	{
		if (ta->mPhase == 0.0)
		{
			// start of animation
			ta->mStartTexture = *Env->Screen.Clone();
		}

		if (ta->mPhase < 1.0)
		{
			// only step in animation		
			blend.FillGradient(ta->mGradient, 1.0 - ta->mPhase);
			Env->Screen.BlendFrom(ta->mStartTexture, ta->mTargetTexture, blend, ta->mBlendEffect);

			// go (last) step further ..
			ta->mPhase = std::min(1.0, ta->mPhase + ta->mSpeed * step);
		}
		else
		{
			// animation ended
			Env->Screen.BlitFrom(0, 0, ta->mTargetTexture);

			// remove this animation
			LedAnimation tmp;
			AnimationQueue.dequeue(tmp);
		}
	}

	// restart again?
	if (AnimationQueue.GetLength() < 1)
	{
		AddWelcomeAnimations();
	}
}