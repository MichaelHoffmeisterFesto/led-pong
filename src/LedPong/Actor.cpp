#include "basic.h"
#include "Actor.h"

const char PlayerPhases[] = "QWERTZUI";

const Vec2 Dir2Index[] = { Vec2(-1,0), Vec2(0,+1), Vec2(+1,0), Vec2(0,-1) };

char Player::GetPlayerPhase(Vec2 direction, bool openMouth)
{
	int index = -1;
	for (int i=0; i<4; i++)
		if (direction == Dir2Index[i])
		{
			index = 2*i;
			break;
		}

	if (openMouth)
		index++;

	if (index >= 0 && index < SIZE_OF_ARR(PlayerPhases))
		return PlayerPhases[index];
	else
		return 'Q';
}