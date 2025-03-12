#include <algorithm> // max, min
#include <vector> // for vectors
#include "basic.h"
#include "GameConst.h"
#include "LedAnimation.h"
#include "CQueueDynGeneric.h"

#include "GameBase.h"
#include "HighScoreGame.h"
#include "MenuGame.h"

//
// The following lines are an example of a handcrafted (HC_) data structure
//

struct HighScoreRecord
{
	// 3 letters for the name
	char Name[4];
	int Score;
};

// Singleton for the high score data
const int HC_MaxHighScore = 99;
int HC_NumHighScore = 0;
HighScoreRecord HC_HighScores[HC_MaxHighScore] = { 0 };

// Manage the Singleton
bool HC_AddHighScore(const char* name, int score)
{
	// find the right index
	int insertAt = 0;
	while (insertAt < HC_NumHighScore && HC_HighScores[insertAt].Score > score)
		insertAt++;

	// special case: 100 high scores lower?
	if (insertAt >= HC_MaxHighScore)
		return false;

	// ok, move scores with lower scores down
	for (int i = std::min(HC_NumHighScore - 1, HC_MaxHighScore - 2); i >= insertAt; i--)
	{
		HC_HighScores[i + 1] = HC_HighScores[i];
	}
}

//
// The following lines are an example of using a pre defined data structure (PD_)
//

// Singleton for the high score data
std::vector<HighScoreRecord> PD_HighScores;

// Manage the Singleton
bool PD_AddHighScore(const char* name, int score)
{
	// find the right place
	std::vector<HighScoreRecord>::iterator it;
	std::vector<HighScoreRecord>::iterator insertAt = PD_HighScores.begin();
	for (it = PD_HighScores.begin(); it < PD_HighScores.end(); it++)
	{
		if (it->Score > score)
			// insertAt position needs to be updated (to the right = lower ranks)
			insertAt = it;
		else
			break;
	}

	// make new
	HighScoreRecord hsr;
	strcpy(hsr.Name, name);
	hsr.Score = score;

	// insert
	PD_HighScores.insert(insertAt, hsr);

	// ok
	return true;
}

HighScoreGame::HighScoreGame(GameEnvironment* env) : GameBase(env)
{
	// Ressources
	// PageBallons = LedTexture("media/ballons.bmp");
}

HighScoreGame::~HighScoreGame()
{
}

void HighScoreGame::Loop()
{
	// keep sound playing
	if (!Env->SoundIsPlaying)
		Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_IntroMusic;

	// go further?
	if (Env->GameKey[KEY_ANY])
	{
		// this pointer will be passed ..
		Env->SoundAllStop = true;
		NextGame = new MenuGame(Env);
	}
}