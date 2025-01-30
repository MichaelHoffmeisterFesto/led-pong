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
#include "Game.h"

// provide instances for extern symbols
LedColorsTable LedColors;

using namespace std;

//
// technology neutral game
//

Game TheGame;

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
	SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, 
	SDL_SCANCODE_A, SDL_SCANCODE_D, SDL_SCANCODE_W, SDL_SCANCODE_S
};

GameKeyEnum GameKeyToMap[] = { 
	KEY_P1_LEFT, KEY_P1_RIGHT, KEY_P1_UP, KEY_P1_DOWN,
	KEY_P2_LEFT, KEY_P2_RIGHT, KEY_P2_UP, KEY_P2_DOWN,
};

// sound samples

SDL_SoundSample* SoundSamples[GameSoundSampleEnum::SOUND_SAMPLE_MAX_NUM] = { nullptr };

void LoadSoundSamples()
{
	SoundSamples[GameSoundSampleEnum::EnergyPill] = new SDL_SoundSample("media/arcade-fx-288597_shorted_fade_out.wav", MIX_MAX_VOLUME);
}

void PlaySoundSample(GameSoundSampleEnum ss)
{
	if (SoundSamples[ss] != nullptr)
		SoundSamples[ss]->play();
}

void DeleteSoundSamples()
{
	for (int i = 0; i < GameSoundSampleEnum::SOUND_SAMPLE_MAX_NUM; i++)
		if (SoundSamples[i] != nullptr)
			delete SoundSamples[i];
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

		if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_P)
		{
		}

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

	for (int i = 0; i < SIZE_OF_ARR(SdlScanToMap); i++)
		TheGame.GameKey[i] = keys[SdlScanToMap[i]];

	TheGame.Loop();

	PlaySoundSample(TheGame.SoundSampleToPlay);
	TheGame.SoundSampleToPlay = GameSoundSampleEnum::None;

	//
	// Display
	//

	for (int x = 0; x < WALL_Xdim; x++)
	{
		for (int y = 0; y < WALL_Ydim; y++)
		{
			// no object allocation, please!
			LedColor* lp = &TheGame.Screen[y][x];
			SDL_Color col = { lp->R, lp->G, lp->B };
			SDL_SetRenderDrawColor(Main->Renderer, &col);

			SDL_Rect wallrect = { 
				14 + (WALL_XpixSize + WALL_Xgap) * x,
				14 + (WALL_YpixSize + WALL_Ygap) * y, 
				WALL_XpixSize, WALL_YpixSize };
			SDL_RenderFillRect(Main->Renderer, &wallrect);
		}
	}

	// side panel

	int spX = 14 + (WALL_Xdim + 2) * (WALL_XpixSize + WALL_Xgap);
	int spY = 14;

	char buffer[100];
	sprintf_s(buffer, "Frame %02d", TheGame.FrameCounter);
	SDL_RenderDrawText(Main->Renderer, Main->FontNormal, SDL_Color_From(LedColors.White), spX, spY, buffer);
	TheGame.FrameCounter = (TheGame.FrameCounter + 1) % 60;

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
	srand(time(0));

	Main = new SDL_Application();

	if (!Main->init(800, 670)) return 1;

	//printf("Music Error %s", SDL_GetError());

	//int total = Mix_GetNumChunkDecoders();
	//for (int i = 0; i < total; i++) {
	//	printf(" - chunk decoder: %s\n", Mix_GetChunkDecoder(i));
	//}

	//total = Mix_GetNumMusicDecoders();
	//for (int i = 0; i < total; i++) {
	//	printf(" - music decoder: %s\n", Mix_GetMusicDecoder(i));
	//}

	LoadSoundSamples();

	SDL_SoundSample smp1("media/arcade-fx-288597_shorted_fade_out.wav", MIX_MAX_VOLUME);
	smp1.play();


	while (loop()) {
		// wait before processing the next frame
		SDL_Delay(1000 / GAME_FrameRate);
	}

	Main->kill();

	DeleteSoundSamples();

	return 0;
}