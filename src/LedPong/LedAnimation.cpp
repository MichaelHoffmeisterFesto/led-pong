#include "LedAnimation.h"

LedAnimation::LedAnimation()
{
	mGradient = LedGradient::LeftToRight;
	mBlendEffect = LedBlendEffect::No;
}

LedAnimation::LedAnimation(LedTexture targetTexture, double speed,
	LedGradient gradient, LedBlendEffect blendEffect)
	: mTargetTexture(targetTexture)
{
	mPhase = 0.0;
	mSpeed = speed;
	mGradient = gradient;
	mBlendEffect = blendEffect;
}

//LedAnimationQueue::LedAnimationQueue() : CQueueDynGeneric()
//{
//	// need to call the base constructor
//}
//
//LedAnimationQueue::~LedAnimationQueue()
//{
//	// base class destructor is automatically called
//}
