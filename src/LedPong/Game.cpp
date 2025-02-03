#include "Game.h"

#include <float.h> // for DBL_MAX ..

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
		"QWERTZUIABCD.-|HJKLtb=lr#jhokasdf uvwx*ceg$§");

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
	tm1->GhostHomeZone[0] = Vec2(3, 3);
	tm1->GhostHomeZone[1] = Vec2(3, 18);
	tm1->GhostHomeZone[2] = Vec2(13, 3);
	tm1->GhostHomeZone[3] = Vec2(14, 21);
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
	LoadLevel(tm1, 1);

	// start with welcome
	AddWelcomeAnimations();
}	

void Game::LoadLevel(TileMap* level, int levelNo, bool resetPlayer)
{
	// remember
	LevelCurr = level;
	Run.LevelNo = levelNo;

	// reset various states
	Run.PillsAvailable = LevelCurr->PillsTotal;

	// first the game is in scatter
	Run.GhostsMode = GhostMode::Scatter;
	Run.CountDownToChase = 7 * GAME_FrameRate;

	// determine speed of pac and ghost
	// see: https://www.reddit.com/r/Pacman/comments/1cg2ogp/does_anyone_know_the_pixel_per_frame_speeds_of/
	// see: https://pacman.holenet.info/#LvlSpecs
	double phaseStepPacNorm = GAME_PacMan_Phase_per_Frame;
	double phaseStepPacFright = GAME_PacMan_Phase_per_Frame;
	double phaseStepGhostNorm = GAME_PacMan_Phase_per_Frame;
	double phaseStepGhostFright = GAME_PacMan_Phase_per_Frame;
	switch (levelNo)
	{
		case 1:
			phaseStepPacNorm *= 1.0;
			phaseStepPacFright *= 1.25;
			phaseStepGhostNorm *= 0.9375;
			phaseStepGhostFright *= 0.625;
			break;

		case 2:
		case 3:
		case 4:
			phaseStepPacNorm *= 1.25;
			phaseStepPacFright *= 1.1875;
			phaseStepGhostNorm *= 1.0625;
			phaseStepGhostFright *= 0.6875;
			break;

		default:
			phaseStepPacNorm *= 1.25;
			phaseStepPacFright *= 1.25;
			phaseStepGhostNorm *= 1.1875;
			phaseStepGhostFright *= 0.75;
			break;
	}

	for (int i = 0; i < SIZE_OF_ARR(Players); i++)
	{
		if (resetPlayer)
			Players[i]->CurrentTilePosition = LevelCurr->PlayerStartPos[i];
		Players[i]->PhaseStepNorm = phaseStepPacNorm;
		Players[i]->PhaseStepFright = phaseStepPacFright;
	}

	for (int i = 0; i < SIZE_OF_ARR(Ghosts); i++)
	{
		if (Ghosts[i] == nullptr)
			continue;
		Ghosts[i]->ResetToInitialPosition(LevelCurr->GhostStartPos[i]);
		Ghosts[i]->HomeZone = LevelCurr->GhostHomeZone[i];
		Ghosts[i]->PhaseStepNorm = phaseStepGhostNorm;
		Ghosts[i]->PhaseStepFright = phaseStepGhostFright;
	}

	// reset fruits
	SpawnedItem dummy;
	while (nullptr != SpawnedItems.top())
		SpawnedItems.dequeue(dummy);
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
	if (false)
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
	}
	else if (Run.CountdownPacManDead > 0)
	{
		//
		// Dead Pac Man
		//

		LevelCurr->DrawMap(Run, Screen, Vec2(0, 0), TrFs, 5, 5);

		for (int pni = 0; pni < std::max(1, std::min(2, Run.NumPlayer)); pni++)
		{
			char xx[] = { Players[pni]->GetDeadPlayerAvatar(Run.SpecialAnimPhase), '\0'};
			TrFs.DrawTextTo(
				Screen, Players[pni]->GetCurrentPixelPos(Vec2(5, 5)),
				xx
			);
		}

		for (int gni = 0; gni < std::max(1, std::min(4, Run.NumGhost)); gni++)
		{
			char xx[] = { Ghosts[gni]->GetGhostAvatarChar(Run, Ghosts[gni]->CurrentDirection), '\0'};
			Vec2 xy = Run.SpecialAnimGhostDelta[gni] * (5.0 * Run.SpecialAnimPhase);
			printf("%d %d\n", xy.X, xy.Y);
			TrFs.DrawTextTo(
				Screen, Ghosts[gni]->GetCurrentPixelPos(Vec2(5, 5)) + xy,
				xx
			);
		}

		Run.SpecialAnimPhase += Run.SpecialAnimStep;

		Run.CountdownPacManDead--;

		if (Run.CountdownPacManDead == 0)
		{
			// finally
			LoadLevel(LevelCurr, Run.LevelNo, false);
		}
	}
	else 
	{
		//
		// normal gameplay
		//

		// some runtime
		char buffer[10];

		// increment pure game level run state
		Run.Animate();

		// not very elegantly programmed: access some of the game-run counters to release some events
		if (Run.FrightenedCounter == 2)
		{
			Run.SetMessage("CHASE");
			SoundSampleToPlay = GameSoundSampleEnum::TurnFromGhosts;
			Run.CountDownToScatter = 10 * GAME_FrameRate;
		}

		if (Run.CountDownToChase == 1 && Run.GhostsMode == GhostMode::Scatter)
		{
			Run.GhostsMode = GhostMode::Chase;
			Run.SetMessage("CHASE");
			SoundSampleToPlay = GameSoundSampleEnum::TurnFromGhosts;
			Run.CountDownToScatter = 20 * GAME_FrameRate;
		}

		if (Run.CountDownToScatter == 1 && Run.GhostsMode == GhostMode::Chase)
		{
			Run.GhostsMode = GhostMode::Scatter;
			Run.SetMessage("SCATR");
			SoundSampleToPlay = GameSoundSampleEnum::TurnFromGhosts;
			Run.CountDownToChase = 5 * GAME_FrameRate;
		}

		// the basic map is drawn every frame!
		LevelCurr->DrawMap(Run, Screen, Vec2(0, 0), TrFs, 5, 5);

		//
		// Player 1/2
		//

		for (int pni = 0; pni < std::max(1, std::min(2, Run.NumPlayer)); pni++)
		{
			// more explicit
			Player* pptr = Players[pni];

			// draw player itself
			char xx[] = { pptr->GetPlayerAvatarChar(Run, pptr->CurrentDirection), '\0' };
			TrFs.DrawTextTo(
				Screen, pptr->GetCurrentPixelPos(Vec2(5, 5)),
				xx
			);

			pptr->Animate(Run);

			// already moved?
			bool alreadyMoved = false;
			bool newFinalPosition = false;

			// Translate the keyboard into an direction in a way, that a keypress WITH a 
			// possible direction wins
			Vec2 tmpDirs[] = { Vec2(-1,0), Vec2(+1,0), Vec2(0,-1), Vec2(0,+1) };
			GameKeyEnum tmpScans[] = { 
				KEY_P1_LEFT, KEY_P1_RIGHT, KEY_P1_UP, KEY_P1_DOWN,
				KEY_P2_LEFT, KEY_P2_RIGHT, KEY_P2_UP, KEY_P2_DOWN 
			};

			for (int di = 0; di < SIZE_OF_ARR(tmpDirs); di++)
			{
				PossibleMove pm;
				if (LevelCurr->FindPossibleMoveInDir(&Player1, pptr->CurrentTilePosition, tmpDirs[di], pm)
					&& pptr->StandStill()
					&& GameKey[tmpScans[di + 4*pni]])
				{
					// initiate new movement
					if (pm.JumpMove)
					{
						// jump to new final position
						pptr->CurrentTilePosition = pm.FinalPosition;
						newFinalPosition = true;
					}
					else
					{
						pptr->CurrentDirection = tmpDirs[di];
						pptr->TriggerOpenMouth();
						pptr->MakeStep(Run);
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
			if (!alreadyMoved && !pptr->StandStill())
			{
				// move
				pptr->MakeStep(Run);

				if (pptr->StandStill())
				{
					newFinalPosition = true;
				}
			}

			// NEW final position?
			if (newFinalPosition)
			{
				// finally arrived on NEW current position
				// check ..
				Tile* ct = LevelCurr->GetAsTile(pptr->CurrentTilePosition);
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
							// see: https://pacman.holenet.info/#LvlSpecs
							Run.FrightenedCounter = std::max(2, (7 - Run.LevelNo)) * GAME_FrameRate;
							Run.SetMessage("SCARE");
						}
						else if (ct->IsFruit())
						{
							pptr->Score += 100;
							SoundSampleToPlay = GameSoundSampleEnum::Fruit;
							Run.SetMessage("100UP");
						}
						else if (ct->IsEnergyPill())
						{
							pptr->Score += 1;
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
			if (LevelCurr->PlayerTextScorePos[pni].IsValid)
			{
				if (pptr->Score <= 9999)
					sprintf_s(buffer, "%04d", pptr->Score);
				else
				{
					if (Run.TextSwitch == 0)
						sprintf_s(buffer, "%03dK", pptr->Score / 1000);
					else
						sprintf_s(buffer, "K%03d", pptr->Score % 1000);
				}
				TrPt.DrawTextTo(Screen, LevelCurr->PlayerTextScorePos[pni] * 5, buffer, 1, 0);
			}

			if (LevelCurr->PlayerTextExtraPos[pni].IsValid)
			{
				sprintf_s(buffer, "%1d", 1 + pni);
				TrPt.DrawTextTo(Screen, LevelCurr->PlayerTextExtraPos[pni] * 5, buffer, 0, 0);

				strcpy_s(buffer, "\0\0\0\0");
				for (int i = 0; i < 3; i++)
					if (i < pptr->Lives)
						buffer[i] = 'W';
				TrFs.DrawTextTo(Screen, LevelCurr->PlayerTextExtraPos[pni] * 5 + Vec2(5, 0), buffer, 0, 0);
			}
		}
	
		//
		// Ghost 1..4
		//

		// Some ghosts are heading towards a particular player.
		// Always heading towards the winning player .. he he 
		Player* headingToPlayer = Players[0];
		if (Run.NumPlayer > 1 && Player2.Score > Player1.Score)
			headingToPlayer = Players[1];

		for (int gni = 0; gni < std::max(1, std::min(4, Run.NumGhost)); gni++)
		{
			// more explicit
			Ghost* gptr = Ghosts[gni];
			if (gptr == nullptr)
				continue;

			// draw ghost itself
			char xx[] = { gptr->GetGhostAvatarChar(Run, gptr->CurrentDirection), '\0' };
			TrFs.DrawTextTo(
				Screen, gptr->GetCurrentPixelPos(Vec2(5, 5)),
				xx
			);

			gptr->Animate(Run);

			// ghost is actively waiting?
			if (gptr->IsWaiting())
				continue;

			// already moved?
			bool alreadyMoved = false;

			// initiate a new step; basically when the ghost stands still
			if (gptr->StandStill())
			{
				// can device for next direction (yet still invalid)
				Vec2 nextDir;

				// acquire every basic direction
				// right now, no jumps are foreseen
				int numDir = 0;
				Vec2* basicDirs = gptr->GetBasicDirections(numDir);

				// get the current target tile
				gptr->UseTargetPosition = gptr->GetCurrentTargetPos(Run, 
					headingToPlayer->CurrentTilePosition, 
					headingToPlayer->CurrentDirection ,Ghosts);

				// investigate all directions
				double bestDirLen = DBL_MAX;
				int bestDirIndex = -1;
				for (int di = 0; di < numDir; di++)
				{
					// shortcut
					Vec2* bd = &basicDirs[di];

					// exclude (for the most cases) the direction the ghost is coming from
					if (gptr->CurrentDirection != Vec2(0, 0) && (gptr->CurrentDirection * (-1)) == *bd)
						continue;

					// potential next step position
					Vec2 nsp = gptr->CurrentTilePosition + *bd;

					// check if the direction is still moveable?
					Tile* tp = LevelCurr->GetAsTile(nsp);
					if (tp == nullptr || !tp->IsMoveablePosition())
						continue;

					// compute NSP from target
					// Note: currently the home zone
					double l = (nsp - gptr->UseTargetPosition).Length();

					// store as best?
					if (l < bestDirLen)
					{
						bestDirLen = l;
						bestDirIndex = di;
					}
				}

				// found?
				if (bestDirIndex >= 0)
				{
					// found!
					nextDir = basicDirs[bestDirIndex];
				}
				else
				{
					// REVERSE possible?
					if (gptr->CurrentDirection != Vec2(0, 0))
					{
						// REVERSE
						nextDir = gptr->CurrentDirection * (-1);
					}
				}

				// delete basic dirs
				delete[] basicDirs;

				// initiate this dir
				if (nextDir.IsValid)
				{
					gptr->CurrentDirection = nextDir;
					gptr->MakeStep(Run);
					alreadyMoved = true;
				}
			}

			// not already moved
			if (!alreadyMoved && !gptr->StandStill())
			{
				// move
				gptr->MakeStep(Run);

				if (gptr->StandStill())
				{
					// check new position
					Player* foundPlayer = nullptr;
					for (int pni = 0; pni < std::max(1, std::min(2, Run.NumPlayer)); pni++)
						if (Players[pni]->CurrentTilePosition == gptr->CurrentTilePosition)
						{
							foundPlayer = Players[pni]; 
							// the 2nd Player on the same tile will have good luck ;-)
							break;
						}

					// player found .. kill it :-/
					if (foundPlayer != nullptr)
					{
						if (foundPlayer->Lives < 1)
						{
							exit(0);
						}
						else
						{
							foundPlayer->Lives--;
							Run.CountdownPacManDead = GAME_Frames_of_death;
							Run.SpecialAnimPhase = 0.0;
							Run.SpecialAnimStep = 1.0 / GAME_Frames_of_death;

							for (int i = 0; i < SIZE_OF_ARR(Ghosts); i++)
								Run.SpecialAnimGhostDelta[i] = LevelCurr->GhostStartPos[i] - Ghosts[i]->CurrentTilePosition;

							Run.SetMessage("DEAD");
							SoundSampleToPlay = GameSoundSampleEnum::PacManDead;
						}
					}
				}
			}
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