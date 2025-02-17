#pragma once

#include "CQueueDynGeneric.h"
#include "LedTexture.h"

class LedAnimation
{
public:
	LedAnimation();
	LedAnimation(LedTexture targetTexture, double speed, 
		LedGradient gradient, LedBlendEffect blendEffect,
		int standStillFrames = 0,
		void *lambdaData = nullptr,
		int (*lambdaFunc)(void *, int) = nullptr);

public:
	LedTexture mStartTexture;
	LedTexture mTargetTexture;
	double mPhase = 0.0; // 0.0 means starting, no start texture .., 1.0 means ended
	double mSpeed = 1.0; // 1.0 = normal speed
	LedGradient mGradient;
	LedBlendEffect mBlendEffect;
	int mStandStillFrames = 0;
	void* mLambdaData = nullptr;
	int (*mLambdaFunc)(void *, int) = nullptr;
};
