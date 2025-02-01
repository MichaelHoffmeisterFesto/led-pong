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
		"QWERTZUIABCD.-|HJKLtb=lr#jhokasdf uvwx*ceg");

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
		"|.L-H.L-H.L-----H.|",
		"|.| |.| |.|     |.|",
		"|.K-J.K-J.K-----J.|",
		"|..*...........*..|",
		"|.L-H.L-----H.L-H.|",
		"|.K-J.K-----J.K-J.|",
		"|.................|",
		"K---H.L-H.L-H.L---J",
		"    |.|LJ.KH|.|    ",
		"----J.||...||.K----",
		"......||...||......",
		"----H.||...||.L----",
		"    |.|KH.LJ|.|    ",
		"L---J.K-J.K-J.K---H",
		"|*...............*|",
		"|.L-H.L-H.L-H.L-H.|",
		"|.| |.u-J.u-J.| |.|",
		"|.K-J.|...|...| |.|",
		"|.....|.--J.L-J |.|",
		"|.L-H.|.....|   |.|",
		"|.K-J.K-----w---J.|",
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
	tm1->PlayerTextScorePos[0] = Vec2(0, 10);
	tm1->PlayerTextScorePos[1] = Vec2(0, 14);
	tm1->PlayerTextExtraPos[0] = Vec2(15, 10);
	tm1->PlayerTextExtraPos[1] = Vec2(15, 14);
	tm1->MessagePos = Vec2(11, 3);
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
	Run.PillsAvailable = LevelCurr->PillsTotal;
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
		// increment pure game level run state
		Run.Animate();

		// the basic map is drawn every frame!
		LevelCurr->DrawMap(Run, Screen, Vec2(0, 0), TrFs, 5, 5);

		//
		// Player 1
		//

		// draw player itself
		char xx[] = { Player1.GetPlayerAvatarChar(Run, Player1.CurrentDirection), '\0' };
		TrFs.DrawTextTo(
			Screen, Player1.GetCurrentPixelPos(Vec2(5, 5)),
			xx
		);

		Player1.Animate();

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
					Player1.TriggerOpenMouth();
					Player1.MakeStep();
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
			Player1.MakeStep();

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
			if (ct != nullptr)
			{
				if (ct->IsEatablePosition())
				{
					// now differentiate
					if (ct->IsEnergizer(Run))
					{
						// turn to ghosts!
						// TODO!
						SoundSampleToPlay = GameSoundSampleEnum::TurnToGhosts;
						Run.SetMessage("SCARE");
					}
					else
					if (ct->IsFruit())
					{					
						Player1.Score += 100;
						SoundSampleToPlay = GameSoundSampleEnum::Fruit;
						Run.SetMessage("100UP");
					}
					else
					if (ct->IsEnergyPill())
					{
						Player1.Score += 1;
						SoundSampleToPlay = GameSoundSampleEnum::EnergyPill;
					}
					
					// pill consumed in any case
					// change to empty
					ct->mTileCode = ' ';

					// also count as consumed pill in any case
					if (Run.PillsAvailable > 0)
						Run.PillsAvailable--;
				}
				else
				{
					// empty place
					// release a (smaller) sound
					SoundSampleToPlay = GameSoundSampleEnum::EmptyTile;
				}
			}
		}

		// player stats
		char buffer[10];
		if (LevelCurr->PlayerTextScorePos[0].IsValid)
		{
			if (Player1.Score <= 9999)
				sprintf_s(buffer, "%04d", Player1.Score);
			else
			{
				if (Run.TextSwitch == 0)
					sprintf_s(buffer, "%03dK", Player1.Score / 1000);
				else
					sprintf_s(buffer, "K%03d", Player1.Score % 1000);
			}
			TrPt.DrawTextTo(Screen, LevelCurr->PlayerTextScorePos[0] * 5, buffer, 1, 0);
		}

		if (LevelCurr->PlayerTextExtraPos[0].IsValid)
		{
			strcpy_s(buffer, "1");
			TrPt.DrawTextTo(Screen, LevelCurr->PlayerTextExtraPos[0] * 5, buffer, 0, 0);

			strcpy_s(buffer, "\0\0\0\0");
			for (int i = 0; i < 3; i++)
				if (i < Player1.Lives)
					buffer[i] = 'W';
			TrFs.DrawTextTo(Screen, LevelCurr->PlayerTextExtraPos[0] * 5 + Vec2(5, 0), buffer, 0, 0);
		}

		//
		// Further items
		//

		// message
		if (Run.MsgLifeTime > 0 && LevelCurr->MessagePos.IsValid)
		{
			strcpy_s(buffer, Run.Message);
			TrPt.DrawTextTo(Screen, LevelCurr->MessagePos * 5, buffer, 0, 0);
		}

		// monitor and remove spawned items
		SpawnedItem* spiTop = SpawnedItems.top();
		if (spiTop != nullptr)
		{
			if (spiTop->FrameCountdown > 0)
				spiTop->FrameCountdown--;
			else
			{
				Tile* tp = LevelCurr->GetAsTile(spiTop->Position);
				if (tp != nullptr && tp->IsFruit())
					tp->mTileCode = '.';
				SpawnedItem dummy;
				SpawnedItems.dequeue(dummy);
			}
		}

		// at an arbitrary frame time, spawn a new goody ..
		if (rand() % 1000 < GAME_Promille_to_spawn_fruit)
		{
			Vec2 pos = LevelCurr->FindRandomFreeEnergyPillPosition();
			Tile* tp = LevelCurr->GetAsTile(pos);
			if (pos.IsValid && tp != nullptr)
			{
				// we have three fruits to choose from
				char fruits[] = { 'c', 'e', 'g' };
				char fruit = fruits[rand() % 3];

				// set and trace
				tp->mTileCode = fruit;
				SpawnedItem spi(pos, GAME_Spawned_item_base_frames + rand() % GAME_Spawned_item_frame_range);
				SpawnedItems.enqueue(spi);
			}
		}
	}

}