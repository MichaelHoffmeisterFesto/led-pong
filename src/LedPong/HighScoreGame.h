#include <algorithm> // max, min
#include "basic.h"
#include "GameConst.h"
#include "GameBase.h"

class HighScoreGame : public GameBase
{
	HighScoreGame(GameEnvironment* env);
	~HighScoreGame();

	// Repeatedly called by the main application
	virtual void Loop();

private:
	// ressources
	// LedTexture PageBallons;

};

