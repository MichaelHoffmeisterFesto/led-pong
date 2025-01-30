#include "Game.h"

Game::Game()
{
	// Ressources
	PageBallons = LedTexture("media/ballons.bmp");
	PageFunnyCar = LedTexture("media/ai_funny_car.bmp");
	PageInfo2 = LedTexture("media/HKA_Info2.bmp");

	// screen
	Screen = LedTexture(WALL_Xdim, WALL_Ydim);

	// renderers
	TrFs = TextRendererFixedSize(
		"media/pacman-kacheln5x5.bmp", 5, 1,
		"QWERTZUIABCD.-|HJKLtb=lr#jhokasdf uvwx");

	ProportionalTextGlyph glyphs[] = {
		{ 'A', 0, 5 },
		{ 'B', 6, 5 },
		{ 'C', 12, 5 },
		{ 'D', 18, 5 },
		{ 'E', 24, 5 },
		{ 'F', 30, 5 },
		{ 'G', 36, 5 },
		{ 'H', 42, 5 },
		{ 'I', 48, 5 },
		{ 'J', 54, 4 },
		{ 'K', 59, 4 },
		{ 'L', 64, 4 },
		{ 'M', 69, 5 },
		{ 'N', 75, 5 },
		{ 'O', 81, 5 },
		{ 'P', 87, 5 },
		{ 'Q', 93, 5 },
		{ 'R', 99, 5 },
		{ 'S', 105, 5 },
		{ 'T', 111, 5 },
		{ 'U', 117, 5 },
		{ 'V', 123, 5 },
		{ 'W', 129, 5 },
		{ 'X', 135, 5 },
		{ 'Y', 141, 5 },
		{ 'Z', 147, 5 },
		{ '0', 153, 4 },
		{ '1', 158, 4 },
		{ '2', 163, 4 },
		{ '3', 168, 4 },
		{ '4', 173, 4 },
		{ '5', 178, 4 },
		{ '6', 183, 4 },
		{ '7', 188, 4 },
		{ '8', 193, 4 },
		{ '9', 198, 4 },
		{ '-', 203, 3 },
		{ '_', 207, 3 },
		{ '.', 211, 3 },
		{ ',', 214, 4 },
		{ ' ', 218, 3 }
	};

	TrPt = TextRendererProportionalText("media/charset-prop-5x5.bmp", 
		SIZE_OF_ARR(glyphs), glyphs);

	// levels
	string x1[] = {
		"L-----------------H",
		"|.................|",
		"|.LH.L-H.L------H.|",
		"|.||.| |.|      |.|",
		"|.KJ.K-J.K------J.|",
		"|.................|",
		"|.LH.L-------H.LH.|",
		"|.KJ.K-------J.KJ.|",
		"|.................|",
		"K--H.L--H.L--H.L--J",
		"   |.| LJ.KH |.|   ",
		"---J.| |...| |.K---",
		".....| |...| |.....",
		"---H.| |...| |.L---",
		"   |.| KH.LJ |.|   ",
		"L--J.K--J.K--J.K--H",
		"|.................|",
		"|.LH.L--H.L--H.LH.|",
		"|.||.u--J.u--J.||.|",
		"|.KJ.|....|....||.|",
		"|....|..--J.L--J|.|",
		"|.LH.|......|   |.|",
		"|.KJ.K------w---J.|",
		"|.................|",
		"K-----------------J"
	}; 
	TileMap* tm1 = new TileMap(SIZE_OF_ARR(x1), x1);
	tm1->GateToLeft = Vec2(0, 12);
	tm1->GateToRight = Vec2(18, 12);
	tm1->PlayerStartPos[0] = Vec2(9, 21);
	tm1->PlayerStartPos[1] = Vec2(9, 5);
	tm1->GhostStartPos[0] = Vec2(8, 11);
	tm1->GhostStartPos[1] = Vec2(10, 11);
	tm1->GhostStartPos[2] = Vec2(8, 13);
	tm1->GhostStartPos[3] = Vec2(10, 13);
	Levels[LevelNum++] = tm1;

	string x2[] = {
		"L--------H|.|L----H",
		"|........||.||....|",
		"|.L----H.KJ.KJ.LH.|",
		"|.|    |.......KJ.|",
		"|.K----J.L-H......|",
		"|........K-J.L--H.|",
		"|.L--H.......|  |.|",
		"|.K--w-------w--J.|",
		"|.................|",
		"K--H.L--H.L--H.L--J",
		"   |.| LJ.KH |.|   ",
		"---J.| |...| |.K---",
		".....| |...| |.....",
		"---H.| |...| |.L---",
		"   |.| KH.LJ |.|   ",
		"L--J.K--J.K--J.K--H",
		"|.................|",
		"|.LH.LH.L-x----xH.|",
		"|.||.||.K-J....||.|",
		"|.||.||.....LH.||.|",
		"|.|| Kw-----wJ.KJ.|",
		"|.||..............|",
		"|.KJ.LH.LH.LH.L-H.|",
		"|....||.||.||.| |.|",
		"K----J|.|K-ww-w-w-J"
	};
	TileMap* tm2 = new TileMap(SIZE_OF_ARR(x2), x2);
	Levels[LevelNum++] = tm2;

	// load level
	LoadLevel(tm1);

	// start with welcome
	AddWelcomeAnimations();
}	

void Game::LoadLevel(TileMap* level)
{
	// remember
	LevelCurr = level;

	// reset various states
	Player1.CurrentTilePosition = LevelCurr->PlayerStartPos[0];
	LevelCurr->PillsAvailable = LevelCurr->PillsTotal;
}

Game::~Game()
{
	for (int i = 0; i < LevelNum; i++)
		delete Levels[i];
}

void Game::AddWelcomeAnimations()
{
	AnimationQueue.enqueue(LedAnimation(
		PageInfo2, 1.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect()));

	AnimationQueue.enqueue(LedAnimation(
		PageFunnyCar, 1.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect()));

	AnimationQueue.enqueue(LedAnimation(
		PageBallons, 2.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect()));
}

void Game::Loop()
{
	double step = 0.01;

	auto ta = AnimationQueue.top();
	if (false && ta != nullptr)
	{
		if (ta->mPhase == 0.0)
		{
			// start of animation
			ta->mStartTexture = Screen.Clone();
		}

		if (ta->mPhase < 1.0)
		{
			// only step in animation
			auto blend = Screen.Clone();
			blend.FillGradient(ta->mGradient, ta->mPhase);
			Screen.BlendFrom(ta->mStartTexture, ta->mTargetTexture, blend, ta->mBlendEffect);

			// go (last) step further ..
			ta->mPhase = std::min(1.0, ta->mPhase + ta->mSpeed * step);
		}
		else
		{
			// animation ended
			Screen.BlitFrom(0, 0, ta->mTargetTexture);

			// remove this animation
			LedAnimation tmp;
			AnimationQueue.dequeue(tmp);
		}
	}

	// restart again?
	if (false && AnimationQueue.GetLength() < 1)
	{
		AddWelcomeAnimations();
	}

	// test
	if (true)
	{
		LevelCurr->DrawMap(Screen, Vec2(0, 0), TrFs, 5, 5);

		bool openMouth = false;
		if (openMouthTime > 0)
		{
			openMouth = (FrameCounter % 6) >= 3;
			openMouthTime--;
		}

		char xx[] = { Player1.GetPlayerPhase(Player1.CurrentDirection, openMouth), '\0' };
		TrFs.DrawTextTo(
			Screen, Player1.GetCurrentPixelPos(Vec2(5, 5)),
			xx
		);

		// already moved?
		bool alreadyMoved = false;
		bool newFinalPosition = false;

		// Translate the keyboard into an direction in a way, that a keypress WITH a 
		// possible direction wins
		Vec2 tmpDirs[] = { Vec2(-1,0), Vec2(+1,0), Vec2(0,-1), Vec2(0,+1) };
		GameKeyEnum tmpScans[] = { KEY_P1_LEFT, KEY_P1_RIGHT, KEY_P1_UP, KEY_P1_DOWN };

		for (int di = 0; di < SIZE_OF_ARR(tmpDirs); di++)
		{
			PossibleMove pm;
			if (LevelCurr->FindPossibleMoveInDir(&Player1, Player1.CurrentTilePosition, tmpDirs[di], pm)
				&& Player1.StandStill()
				&& GameKey[tmpScans[di]])
			{
				// initiate new movement
				if (pm.JumpMove)
				{
					// jump to new final position
					Player1.CurrentTilePosition = pm.FinalPosition;
					newFinalPosition = true;
				}
				else
				{
					Player1.CurrentDirection = tmpDirs[di];
					Player1.Animate();
				}

				// no automatic move anymore
				alreadyMoved = true;

				// allow open mouth
				openMouthTime = 6;

				// no more direction
				break;
			}
		}

		// not already moved
		if (!alreadyMoved && !Player1.StandStill())
		{
			// move
			Player1.Animate();

			if (Player1.StandStill())
			{
				newFinalPosition = true;
			}
		}

		// NEW final position?
		if (newFinalPosition)
		{
			// finally arrived on NEW current position
			// check ..
			Tile* ct = LevelCurr->GetAsTile(Player1.CurrentTilePosition);
			if (ct != nullptr && ct->IsEnergyPill())
			{
				// change to empty
				ct->mTileCode = ' ';

				// count
				if (LevelCurr->PillsAvailable > 0)
					LevelCurr->PillsAvailable--;

				printf("remaid %d\n", LevelCurr->PillsAvailable);

				Player1.Score += 1;

				// release a sound
				SoundSampleToPlay = GameSoundSampleEnum::EnergyPill;
			}
		}
	}

}