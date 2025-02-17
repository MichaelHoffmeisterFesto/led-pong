#include "GameBase.h"
#include "PacManGame.h"

#include <float.h> // for DBL_MAX ..
#include "LevelInit.h"

PacManGame::PacManGame(GameEnvironment* env) : GameBase(env)
{
	// level presets (outsourced for clarity)
	LevelInit::InitPresetLevels(LevelNum, Levels);

	// load level
	LoadLevel(0, 1);

	// indicate
	Run.SetMessage("LEV01");
	// Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_LevelWin;
}	

void PacManGame::RestartLevel()
{
	// first the game is in scatter
	Run.GhostsMode = GhostMode::Scatter;
	Run.CountDownToChase = 7 * GAME_FrameRate;

	// reset fruits
	SpawnedItem dummy;
	while (nullptr != SpawnedItems.top())
		SpawnedItems.dequeue(dummy);

	// reset ghosts
	for (int i = 0; i < SIZE_OF_ARR(Ghosts); i++)
	{
		if (Ghosts[i] == nullptr)
			continue;
		Ghosts[i]->ResetToInitialPosition(LevelCurr->GhostStartPos[i]);
	}
}

void PacManGame::LoadLevel(int tileMapIndex, int levelNo)
{
	// remember
	if (tileMapIndex >= 0 && tileMapIndex < LevelNum)
	{
		// delete the old one
		if (LevelCurr != nullptr)
			delete LevelCurr;

		// make new
		LevelCurr = Levels[tileMapIndex]->Clone();
	}

	Run.TileMapIndex = tileMapIndex;
	Run.LevelNo = levelNo;

	// reset various states
	Run.PillsAvailable = LevelCurr->PillsTotal;

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
		if (Players[i] == nullptr)
			continue;
		Players[i]->CurrentTilePosition = LevelCurr->PlayerStartPos[i];
		Players[i]->PhaseStepNorm = phaseStepPacNorm;
		Players[i]->PhaseStepFright = phaseStepPacFright;
	}	

	for (int i = 0; i < SIZE_OF_ARR(Ghosts); i++)
	{
		if (Ghosts[i] == nullptr)
			continue;
		Ghosts[i]->HomeZone = LevelCurr->GhostHomeZone[i];
		Ghosts[i]->PhaseStepNorm = phaseStepGhostNorm;
		Ghosts[i]->PhaseStepFright = phaseStepGhostFright;
	}

	RestartLevel();
}

PacManGame::~PacManGame()
{
	if (LevelCurr != nullptr)
		delete LevelCurr;
	
	for (int i = 0; i < LevelNum; i++)
		delete Levels[i];
}

void PacManGame::Loop()
{
	// some book keeping for independent classes
	Run.FrameCounter = Env->FrameCounter;

	if (Run.CountdownPacOrGhostDead > 0)
	{
		//
		// Dead Pac Man / Ghost
		//

		LevelCurr->DrawMap(Run, Env->Screen, Vec2(0, 0), Env->TrFs, 5, 5);

		// blink player
		for (int pni = 0; pni < std::max(1, std::min(2, Run.NumPlayer)); pni++)
		{
			char xx[] = { Players[pni]->GetDeadPlayerAvatar(Run.SpecialAnimPhase), '\0'};
			Env->TrFs.DrawTextTo(
				Env->Screen, Players[pni]->GetCurrentPixelPos(Vec2(5, 5)),
				xx
			);
		}

		// animate ghosts (if directional vector is not 0/0)
		for (int gni = 0; gni < std::max(1, std::min(4, Run.NumGhost)); gni++)
		{
			char xx[] = { Ghosts[gni]->GetGhostAvatarChar(Run, Ghosts[gni]->CurrentDirection), '\0'};
			Vec2 xy = Run.SpecialAnimGhostDelta[gni] * (5.0 * Run.SpecialAnimPhase);
			Env->TrFs.DrawTextTo(
				Env->Screen, Ghosts[gni]->GetCurrentPixelPos(Vec2(5, 5)) + xy,
				xx
			);
		}

		// count down animation phase
		Run.SpecialAnimPhase += Run.SpecialAnimStep;
		Run.CountdownPacOrGhostDead--;

		// at the end
		if (Run.CountdownPacOrGhostDead == 0)
		{
			if (Run.SpecialResetOnlyGhost >= 0)
			{
				// usally: ghost dead; reset only this
				Ghosts[Run.SpecialResetOnlyGhost]->ResetToInitialPosition(LevelCurr->GhostStartPos[Run.SpecialResetOnlyGhost]);
			}
			else
			{
				// usally: Pac Man dead; restart level
				RestartLevel();
			}
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
			Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_TurnFromGhosts;
			Run.CountDownToScatter = 10 * GAME_FrameRate;
		}

		if (Run.CountDownToChase == 1 && Run.GhostsMode == GhostMode::Scatter)
		{
			Run.GhostsMode = GhostMode::Chase;
			Run.SetMessage("CHASE");
			Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_TurnFromGhosts;
			Run.CountDownToScatter = 20 * GAME_FrameRate;
		}

		if (Run.CountDownToScatter == 1 && Run.GhostsMode == GhostMode::Chase)
		{
			Run.GhostsMode = GhostMode::Scatter;
			Run.SetMessage("SCATR");
			Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_TurnFromGhosts;
			Run.CountDownToChase = 5 * GAME_FrameRate;
		}

		// the basic map is drawn every frame!
		LevelCurr->DrawMap(Run, Env->Screen, Vec2(0, 0), Env->TrFs, 5, 5);

		// some events?
		int playerDead = -1;
		int ghostDead = -1;
		bool advanceNextLevel = false;

		// keys pressed for some options
		if (Env->AllowDebug)
		{
			if (Env->GameKey[KEY_DEBUG] && !Env->WasGameKey[KEY_DEBUG])
				advanceNextLevel = true;

			if (Env->GameKey[KEY_GOD_MODE] && !Env->WasGameKey[KEY_GOD_MODE])
			{
				Env->GodMode = !Env->GodMode;
				sprintf_s(buffer, "GOD %01d", Env->GodMode);
				Run.SetMessage(buffer);
				Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_Fruit;
			}
		}

		//
		// Player 1/2
		//

		for (int pni = 0; pni < std::max(1, std::min(2, Run.NumPlayer)); pni++)
		{
			// more explicit
			Player* pptr = Players[pni];

			// draw player itself
			char xx[] = { pptr->GetPlayerAvatarChar(Run, pptr->CurrentDirection), '\0' };
			Env->TrFs.DrawTextTo(
				Env->Screen, pptr->GetCurrentPixelPos(Vec2(5, 5)),
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
					&& Env->GameKey[tmpScans[di + 4*pni]])
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
						if (ct->IsEnergizerAnyway(Run) /* debug fright mode: || pptr->Score == 3 */)
						{
							// is actively on?
							if (ct->IsEnergizerActive(Run))
							{
								// a little achievement
								pptr->Score += 50;

								// turn to ghosts!
								// TODO!
								Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_TurnToGhosts;
								// see: https://pacman.holenet.info/#LvlSpecs
								Run.FrightenedCounter = std::max(2, (7 - Run.LevelNo)) * GAME_FrameRate;
								Run.SetMessage("SCARE");
							}
							else
							{
								// not on, only 'normal' pill
								pptr->Score += 1;
								Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_EnergyPill;
							}

							// Extension to original Pac Man: refresh energizers?
							char etc = ct->mTileCode;
							ct->mTileCode = ' ';
							// Note: instead of doing book-keeping, count directly; the performance loss
							// is not that often.
							int remain = LevelCurr->CountTileCode(etc);
							if (remain < 1)
							{
								// add for more new energizers on remaining tiles
								for (int i = 0; i < 4; i++)
								{
									Vec2 pos = LevelCurr->FindRandomFreeEnergyPillPosition();
									Tile* tp = LevelCurr->GetAsTile(pos);
									if (pos.IsValid && tp != nullptr)
										tp->mTileCode = etc;
								}
							}
						}
						else if (ct->IsFruit())
						{
							pptr->Score += 100;
							Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_Fruit;
							Run.SetMessage("100UP");
						}
						else if (ct->IsEnergyPill())
						{
							pptr->Score += 1;
							Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_EnergyPill;
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
						Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_EmptyTile;
					}
				}

				// check if ran into ghost
				for (int gni = 0; gni < std::max(1, std::min(4, Run.NumGhost)); gni++)
					if (Ghosts[gni] != nullptr
						&& Ghosts[gni]->CurrentTilePosition == pptr->CurrentTilePosition
						&& !Env->GodMode)
					{
						if (Run.GhostsMode == GhostMode::Freightened || Run.GhostsMode == GhostMode::SuperFreightened)
						{
							// score up the correct player
							pptr->Score += 200;
							ghostDead = gni;
						}
						else
							playerDead = pni;
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
				Env->TrPt.DrawTextTo(Env->Screen, LevelCurr->PlayerTextScorePos[pni] * 5, buffer, 1, 0);
			}

			if (LevelCurr->PlayerTextExtraPos[pni].IsValid)
			{
				sprintf_s(buffer, "%1d", 1 + pni);
				Env->TrPt.DrawTextTo(Env->Screen, LevelCurr->PlayerTextExtraPos[pni] * 5, buffer, 0, 0);

				strcpy_s(buffer, "\0\0\0\0");
				for (int i = 0; i < 3; i++)
					if (i < pptr->Lives)
						buffer[i] = 'W';
				Env->TrFs.DrawTextTo(Env->Screen, LevelCurr->PlayerTextExtraPos[pni] * 5 + Vec2(5, 0), buffer, 0, 0);
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
			Env->TrFs.DrawTextTo(
				Env->Screen, gptr->GetCurrentPixelPos(Vec2(5, 5)),
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

				// just for the case: count the moveable positions
				int movPosis = LevelCurr->CountMoveableDirections(gptr->CurrentTilePosition, numDir, basicDirs);

				// investigate all directions
				double bestDirLen = DBL_MAX;
				int bestDirIndex = -1;
				int preChoseFrightDir = rand() % 4;
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

					// if in fright mode, let the pseudo random number decice
					// Note: Let only decide, if tile is an intersection!
					if ((Run.GhostsMode == GhostMode::Freightened || Run.GhostsMode == GhostMode::SuperFreightened)
						&& movPosis >= 3)
					{
						if (di != preChoseFrightDir)
							continue;
						bestDirIndex = di;
						break;
					}

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
						if (Players[pni]->CurrentTilePosition == gptr->CurrentTilePosition
							&& !Env->GodMode)
						{
							if (Run.GhostsMode == GhostMode::Freightened || Run.GhostsMode == GhostMode::SuperFreightened)
							{
								// score up the correct player
								Players[pni]->Score += 200;
								ghostDead = gni;
							}
							else
								playerDead = pni;

							// the 2nd Player on the same tile will have good luck ;-)
							break;
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
			Env->TrPt.DrawTextTo(Env->Screen, LevelCurr->MessagePos * 5, buffer, 0, 0);
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

		//
		// Event handling
		//

		if (playerDead >= 0)
		{
			if (Players[playerDead]->Lives < 1)
			{
				exit(0);
			}
			else
			{
				Players[playerDead]->Lives--;
				Run.CountdownPacOrGhostDead = GAME_Frames_of_death;
				Run.SpecialResetOnlyGhost = -1;
				Run.SpecialAnimPhase = 0.0;
				Run.SpecialAnimStep = 1.0 / GAME_Frames_of_death;

				for (int i = 0; i < SIZE_OF_ARR(Ghosts); i++)
					Run.SpecialAnimGhostDelta[i] = LevelCurr->GhostStartPos[i] - Ghosts[i]->CurrentTilePosition;

				Run.SetMessage("DEAD");
				Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_PacManDead;
			}
		}

		if (ghostDead >= 0)
		{
			Run.CountdownPacOrGhostDead = GAME_Frames_of_death;
			Run.SpecialResetOnlyGhost = ghostDead;
			Run.SpecialAnimPhase = 0.0;
			Run.SpecialAnimStep = 1.0 / GAME_Frames_of_death;

			for (int i = 0; i < SIZE_OF_ARR(Ghosts); i++)
				Run.SpecialAnimGhostDelta[i] = (ghostDead != i) ? Vec2(0, 0)
				: LevelCurr->GhostStartPos[i] - Ghosts[i]->CurrentTilePosition;

			Run.SetMessage("BUUUH");
			Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_GhostDead;
		}

		if (advanceNextLevel)
		{
			// Start new level
			Run.LevelNo++;
			int tileMapNdx = (Run.LevelNo + 0) % 2;
			LoadLevel(tileMapNdx, Run.LevelNo);

			// Indicate
			sprintf_s(buffer, "LEV%02d", Run.LevelNo);
			Run.SetMessage(buffer);
			Env->SoundSampleToPlay = GameSoundSampleEnum::SMP_LevelWin;
		}
	}

}