#include <algorithm> // max, min
#include "basic.h"
#include "GameConst.h"
#include "LedAnimation.h"
#include "CQueueDynGeneric.h"

#include "GameBase.h"
#include "IntroGame.h"
#include "MenuGame.h"

IntroGame::IntroGame(GameEnvironment* env) : GameBase(env)
{
	// Ressources
	PageBallons = LedTexture("media/ballons.bmp");
	PageFunnyCar = LedTexture("media/ai_funny_car.bmp");
	PageInfo2 = LedTexture("media/HKA_Info2.bmp");
	PageOnePac = LedTexture("media/ai_pac_man_small.bmp");
	PageManyPac = LedTexture("media/ai_many_pacs_small.bmp");
	PageCredits = LedTexture("media/Credits_small.bmp");

	// first sequence of animations
	AddWelcomeAnimations();
}

IntroGame::~IntroGame()
{
}

struct SlideInData
{
	int FrameNo;
	int FinalFrame;
	int NumEntry;
	int* StartStop;
	const char** Texts;
	GameEnvironment* Env;
};

int CreditsData_StartStop[] = {
	20, 40,
	60, 80,
	100, 120,
	140, 160,
	180, 200,
	220, 240,
	260, 280,
	300, 320
};

const char* CreditsData_Texts[] = {
	"PIX:CHATGPT",
	"SOUNDS:PIXABAY",
	"LUCA DI ",
	"ALESSANDRO + ",
	"SOUNDREALITY +",
	"SERGEQUADRADO",
	"COPYRIGHT:",
	"M.HOFFMEISTER"
};

SlideInData CreditsData = {
	0,
	500, 
	std::min(SIZE_OF_ARR(CreditsData_StartStop) / 2, SIZE_OF_ARR(CreditsData_Texts)),
	CreditsData_StartStop,
	CreditsData_Texts
};

int CreditsSlideIn(void* vp, int frame)
{
	SlideInData* data = static_cast<SlideInData*>(vp);
	if (data == nullptr || data->Env == nullptr)
		return -1;

	if (frame == 500)
	{
		// start again
		data->FrameNo = 0;
	}

	for (int i = 0; i < data->NumEntry; i++)
	{
		int startI = data->StartStop[2 * i + 0];
		int stopI = data->StartStop[2 * i + 1];
		int xpos = 96 + 4;
		if (data->FrameNo < startI)
			xpos = 96 + 4;
		else if (data->FrameNo > stopI)
			xpos = 5;
		else
			xpos = 5 + (int)((1.0 - (1.0 * data->FrameNo - startI) / abs(stopI - startI)) * (1.0 * (96 + 4) - 5));

		data->Env->TrPt.DrawTextTo(data->Env->Screen, Vec2(xpos, 68 + 7 * i), data->Texts[i], 1, 0);
	}
	data->FrameNo++;
	return 0;
}

void IntroGame::AddWelcomeAnimations()
{
	if (rand() % 1000 < 800)
	{
		AnimationQueue.enqueue(LedAnimation(
			PageInfo2, 1.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect(), 2 * GAME_FrameRate));
	}

	if (rand() % 1000 < 300)
	{
		AnimationQueue.enqueue(LedAnimation(
			PageFunnyCar, 1.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect(), 2 * GAME_FrameRate));
	}

	if (rand() % 1000 < 600)
	{
		AnimationQueue.enqueue(LedAnimation(
			PageOnePac, 2.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect(), 2 * GAME_FrameRate));
	}

	if (rand() % 1000 < 500)
	{
		AnimationQueue.enqueue(LedAnimation(
			PageManyPac, 2.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect(), 2 * GAME_FrameRate));
	}

	if (rand() % 1000 < 200)
	{
		AnimationQueue.enqueue(LedAnimation(
			PageBallons, 2.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect(), 2 * GAME_FrameRate));
	}

	if (rand() % 1000 < 700)
	{
		CreditsData.Env = Env;
		AnimationQueue.enqueue(LedAnimation(
			PageCredits, 2.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect(), 500,
			&CreditsData,
			CreditsSlideIn));
	}

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

			// lambda
			if (ta->mLambdaFunc != nullptr)
				ta->mLambdaFunc(ta->mLambdaData, ta->mStandStillFrames);

			// stand still?
			if (ta->mStandStillFrames > 0)
				ta->mStandStillFrames--;
			else
			{
				// remove this animation
				LedAnimation tmp;
				AnimationQueue.dequeue(tmp);
			}
		}
	}

	// restart again?
	if (AnimationQueue.GetLength() < 1)
	{
		AddWelcomeAnimations();
	}

	// keep sound playing
	if (!Env->SoundIsPlaying)
		Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_IntroMusic;

	// go further?
	if (Env->GameKey[KEY_ANY])
	{
		// this pointer will be passed ..
		Env->SoundAllStop = true;
		NextGame = new MenuGame(Env, 0);
	}
}