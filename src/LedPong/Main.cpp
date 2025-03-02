// HSKA - Technische Informatik 2
// Projekt Planetensimulation
// Setup: https://thenumb.at/cpp-course/sdl2/01/vsSetup.html
// Vorlage: https://www.youtube.com/watch?v=WTLPmUHTPqo

#include <algorithm> // max, min
#include <iostream>
#include <stdint.h> // for standard integer types like uint8_t ..
#include <cstdlib> // for srand() and rand()
#include <ctime>   // for time()

#include <SDL.h>
#include <SDL_ttf.h>
#include "SDL_Helper.h"
#include "SDL_Application.h"
#include "SDL_SoundSample.h"

#include "basic.h"

#include "GameBase.h"
#include "IntroGame.h"
#include "PacManGame.h"

// provide instances for extern symbols
LedColorsTable LedColors;

using namespace std;

//
// technology neutral game
//

GameEnvironment TheEnv;
//IntroGame TheIntro(&TheEnv);
//PacManGame TheGame(&TheEnv);

GameBase* CurrentGame = new IntroGame(&TheEnv);

//
// SDL
//

SDL_Application* Main;

/*
 * Choice of helpful commands for SDL
 *

   SDL_Color COLOR_background = { 204, 236, 255 };
   SDL_SetRenderDrawColor(Main->Renderer, &COLOR_background);

   SDL_Rect wallrect = { 200, 400, 10, -100 };
   SDL_RenderFillRect(Main->Renderer, &wallrect);

   SDL_RenderFillCircle(Main->Renderer, x, y, radius);

   sprintf_s(buffer, "value=%04.1f", float_val);
   SDL_RenderDrawText(Main->Renderer, Main->font_normal, COLOR_foreground, 30, 10, buffer);

   SDL_Point pts[] = { { 10, 10 }, { 20, 10 }, { 20, 20 } };
   SDL_RenderDrawLines(Application->Renderer, pts, 3);

   SDL_Texture* texture = SDL_CreateTextureFromSurface(Main->Renderer, Main->ImageHka);
   SDL_Rect dstrect = { 200, 400, 10, -100 };
   SDL_RenderCopy(Main->Renderer, texture, NULL, &dstrect);

*/

//
// CONSTANTS
//

// some colors
const SDL_Color COLOR_background = { 30, 20, 20 };
const SDL_Color COLOR_white = { 255, 255, 255 };

SDL_Scancode SdlScanToMap[] = { 
	SDL_SCANCODE_UNKNOWN,
	SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, 
	SDL_SCANCODE_A, SDL_SCANCODE_D, SDL_SCANCODE_W, SDL_SCANCODE_S,
	SDL_SCANCODE_F8, SDL_SCANCODE_F9, SDL_SCANCODE_F10,
	SDL_SCANCODE_M, SDL_SCANCODE_G
};

GameKeyEnum GameKeyToMap[] = { 
	KEY_ANY,
	KEY_P1_LEFT, KEY_P1_RIGHT, KEY_P1_UP, KEY_P1_DOWN,
	KEY_P2_LEFT, KEY_P2_RIGHT, KEY_P2_UP, KEY_P2_DOWN,
	KEY_DEBUG1, KEY_DEBUG2, KEY_DEBUG3,
	KEY_MUTE, KEY_GOD_MODE
};

// sound samples

SDL_SoundSample* SoundSamples[GameSoundSampleEnum::SMP_MAX_NUM] = { nullptr };

void LoadSoundSamples()
{
	SoundSamples[GameSoundSampleEnum::SMP_EmptyTile] = new SDL_SoundSample("media/pop-268648-shortened-fade-out.wav", MIX_MAX_VOLUME / 4);
	SoundSamples[GameSoundSampleEnum::SMP_EnergyPill] = new SDL_SoundSample("media/arcade-fx-288597_shorted_fade_out.wav", MIX_MAX_VOLUME / 2);
	SoundSamples[GameSoundSampleEnum::SMP_TurnToGhosts] = new SDL_SoundSample("media/arcade-arped-145549-shortened.wav", MIX_MAX_VOLUME);
	SoundSamples[GameSoundSampleEnum::SMP_TurnFromGhosts] = new SDL_SoundSample("media/arcade-ui-18-229517-shortened.wav", MIX_MAX_VOLUME);
	SoundSamples[GameSoundSampleEnum::SMP_Fruit] = new SDL_SoundSample("media/arcade-ui-28-229497-shortened.wav", MIX_MAX_VOLUME);
	SoundSamples[GameSoundSampleEnum::SMP_PacManDead] = new SDL_SoundSample("media/arcade-ui-26-229495-shortened.wav", MIX_MAX_VOLUME);
	SoundSamples[GameSoundSampleEnum::SMP_GhostDead] = new SDL_SoundSample("media/arcade-ui-1-229498-shortened.wav", MIX_MAX_VOLUME);
	SoundSamples[GameSoundSampleEnum::SMP_LevelWin] = new SDL_SoundSample("media/arcade-ui-11-229509-shortened.wav", MIX_MAX_VOLUME);
	SoundSamples[GameSoundSampleEnum::SMP_IntroMusic] = new SDL_SoundSample("media/cool-hip-hop-loop-251857.wav", MIX_MAX_VOLUME / 4);
	SoundSamples[GameSoundSampleEnum::SMP_MenuMusic] = new SDL_SoundSample("media/fast-rocky-loop-251869.wav", MIX_MAX_VOLUME / 4);
}

void PlaySoundSample(GameSoundSampleEnum ss)
{
	if (SoundSamples[ss] != nullptr)
		SoundSamples[ss]->play();
}

void SoundDoAllStop()
{
	Mix_HaltChannel(-1);
}

bool SoundIsSomePlaying()
{
	int playing_chans = Mix_Playing(-1);
	return playing_chans > 0;
}

void DeleteSoundSamples()
{
	for (int i = 0; i < GameSoundSampleEnum::SMP_MAX_NUM; i++)
		if (SoundSamples[i] != nullptr)
			delete SoundSamples[i];
}

void DebugSound()
{
	printf("Music Error %s", SDL_GetError());

	int total = Mix_GetNumChunkDecoders();
	for (int i = 0; i < total; i++) {
		printf(" - chunk decoder: %s\n", Mix_GetChunkDecoder(i));
	}

	total = Mix_GetNumMusicDecoders();
	for (int i = 0; i < total; i++) {
		printf(" - music decoder: %s\n", Mix_GetMusicDecoder(i));
	}
}

//
// Loop (function to be implemented)
//

bool loop() {

	//
	// Background 
	// 

	SDL_SetRenderDrawColor(Main->Renderer, &COLOR_background);
	SDL_RenderClear(Main->Renderer);

	//
	// Event loop
	//

	static const unsigned char* keys = SDL_GetKeyboardState(nullptr);
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0) {

		// actual state change of the key

		// quit?
		if (e.type == SDL_QUIT)
			return false;

		if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_E)
		{
		}

		if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_I)
		{
		}

		if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_O)
		{
		}
	}

	//
	// call technology neutral game core
	//

	bool keyAny = false;
	for (int i = 1; i < SIZE_OF_ARR(SdlScanToMap); i++)
	{
		TheEnv.GameKey[i] = keys[SdlScanToMap[i]];
		keyAny = keyAny || TheEnv.GameKey[i];
	}
	TheEnv.GameKey[KEY_ANY] = keyAny;

	// TheGame.Loop();
	// TheIntro.Loop();
	if (CurrentGame != nullptr)
	{
		CurrentGame->Loop();

		if (CurrentGame->NextGame != nullptr)
		{
			GameBase* oldGame = CurrentGame;
			CurrentGame = CurrentGame->NextGame;
			delete oldGame;
		}
	}

	if (!TheEnv.Mute)
	{
		PlaySoundSample(TheEnv.SoundSampleToPlay);
		TheEnv.SoundSampleToPlay = GameSoundSampleEnum::SMP_None;

		if (TheEnv.SoundAllStop)
		{
			SoundDoAllStop();
			TheEnv.SoundAllStop = false;
		}

		TheEnv.SoundIsPlaying = SoundIsSomePlaying();
	}

	TheEnv.Animate();

	//
	// Display
	//

	for (int x = 0; x < WALL_Xdim; x++)
	{
		for (int y = 0; y < WALL_Ydim; y++)
		{
			// no object allocation, please!
			LedColor* lp = &TheEnv.Screen[y][x];
			SDL_Color col = { lp->R, lp->G, lp->B };
			SDL_SetRenderDrawColor(Main->Renderer, &col);

			SDL_Rect wallrect = { 
				14 + (WALL_XpixSize + WALL_Xgap) * x,
				14 + (WALL_YpixSize + WALL_Ygap) * y, 
				WALL_XpixSize, WALL_YpixSize };
			SDL_RenderFillRect(Main->Renderer, &wallrect);
		}
	}

	// Debug: to be removed!!
	// side panel for pac man??

	PacManGame* IsPmg = dynamic_cast<PacManGame*>(CurrentGame);

	if (IsPmg != nullptr && TheEnv.ShowDebug)
	{
		int spX = 14 + (WALL_Xdim + 2) * (WALL_XpixSize + WALL_Xgap);
		int spY = 14;

		char buffer[100];
		sprintf_s(buffer, "Frame %02d", TheEnv.FrameCounter % GAME_FrameRate);
		SDL_RenderDrawText(Main->Renderer, Main->FontNormal, SDL_Color_From(LedColors.White), spX, spY, buffer);
	}

	// debug this target pos
	if (IsPmg != nullptr && TheEnv.ShowDebug)
	{
		SDL_Color COLOR_debug = { 204, 236, 255 };
		SDL_SetRenderDrawColor(Main->Renderer, &COLOR_debug);

		for (int gni = 0; gni < std::max(1, std::min(4, IsPmg->Run.NumGhost)); gni++)
		{
			// more explicit
			Ghost* gptr = IsPmg->Ghosts[gni];
			if (gptr == nullptr)
				continue;

			Vec2 from = gptr->CurrentTilePosition * (5 * (WALL_XpixSize + WALL_Xgap));
			Vec2 to = gptr->UseTargetPosition * (5 * (WALL_XpixSize + WALL_Xgap));
			SDL_Point pts[] = { { 14 + from.X, 14 + from.Y }, { 14 + to.X, 14 + to.Y } };
			SDL_RenderDrawLines(Main->Renderer, pts, 2);
		}
	}

	//
	// End of loop
	//

	// Update window
	SDL_RenderPresent(Main->Renderer);

	return true;
}

//
// Main
//

int main(int argc, char** args)
{
	// set the randomness
	srand((unsigned int) time(0));

	// init SDL application; fixed size
	Main = new SDL_Application();
	if (!Main->init(510 /* 800 */, 670)) return 1;	

	// init sound
	LoadSoundSamples();

	// primitive command line parsing
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(args[i], "-mute") == 0)
			TheEnv.Mute = !TheEnv.Mute;
		if (strcmp(args[i], "-god") == 0)
			TheEnv.GodMode = !TheEnv.GodMode;
		if (strcmp(args[i], "-debug") == 0)
			TheEnv.AllowDebug = !TheEnv.AllowDebug;
	}

	// main loop
	while (loop()) {
		// wait before processing the next frame
		SDL_Delay(1000 / GAME_FrameRate);
	}

	// end of application
	Main->kill();
	DeleteSoundSamples();
	return 0;
}