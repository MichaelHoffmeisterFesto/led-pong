#include "basic.h"
#include "Actor.h"

const char PlayerPhases[] = "QWERTZUI";

const Vec2 Dir2Index[] = { Vec2(-1,0), Vec2(0,+1), Vec2(+1,0), Vec2(0,-1) };

char Player::GetPlayerAvatarChar(GameRun& run, Vec2 direction)
{
	int index = -1;
	for (int i=0; i<4; i++)
		if (direction == Dir2Index[i])
		{
			index = 2*i;
			break;
		}

	bool openMouth = (run.FrameCounter % 12) >= 6;

	if (OpenMouthTime > 0 && openMouth)
		index++;

	if (index >= 0 && index < (int) SIZE_OF_ARR(PlayerPhases))
		return PlayerPhases[index];
	else
		return 'Q';
}
