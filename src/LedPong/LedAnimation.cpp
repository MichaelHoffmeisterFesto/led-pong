#include "LedAnimation.h"

#include "GameEnvironment.h"

LedAnimation::LedAnimation()
{
	mGradient = LedGradient::GRAD_LeftToRight;
	mBlendEffect = LedBlendEffect::BLEND_No;
}

LedAnimation::LedAnimation(LedTexture targetTexture, double speed,
	LedGradient gradient, LedBlendEffect blendEffect, int standStillFrames,
	void* lambdaData,
	int (*lambdaFunc)(void *, int))
	: mTargetTexture(targetTexture)
{
	mPhase = 0.0;
	mSpeed = speed;
	mGradient = gradient;
	mBlendEffect = blendEffect;
	mStandStillFrames = standStillFrames;
	mLambdaData = lambdaData;
	mLambdaFunc = lambdaFunc;
}
