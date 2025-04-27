// HSKA - Technische Informatik 2
// Projekt Planetensimulation
// Setup: https://thenumb.at/cpp-course/sdl2/01/vsSetup.html
// Vorlage: https://www.youtube.com/watch?v=WTLPmUHTPqo

#include <algorithm> // max, min
#include <iostream>
#include <stdint.h> // for standard integer types like uint8_t ..
#include <cstdlib> // for srand() and rand()
#include <ctime>   // for time()

#include "led-matrix.h"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>

#include "SDL_SoundSample.h"

#include "basic.h"

#include "GameBase.h"
#include "IntroGame.h"
#include "PacManGame.h"

using namespace std;

using rgb_matrix::RGBMatrix;
using rgb_matrix::FrameCanvas;
using rgb_matrix::Canvas;

// provide instances for extern symbols
LedColorsTable LedColors;

// technology neutral game

GameEnvironment TheEnv;
GameBase* CurrentGame = new IntroGame(&TheEnv);

// LED canvas
RGBMatrix *TheLedCanvas = nullptr;
FrameCanvas *OffScreenCanvas = nullptr;

//
// Interrupt handling
// 

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}

//
// CONSTANTS
//

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

	OffScreenCanvas->Fill(0, 0, 0);

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
			LedColor* lp = &TheEnv.Screen[y][x];
    			OffScreenCanvas->SetPixel(y, 3 * 32 - x, lp->R, lp->G, lp->B );
		}
	}

	//
	// End of loop
	//

	return true;
}

int main(int argc, char** args)
{
	// initialize LED
	RGBMatrix::Options defaults;
	defaults.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
	defaults.cols = 64;
	defaults.rows = 32;
	defaults.chain_length = 6;
	defaults.parallel = 1;
	defaults.show_refresh_rate = false;
	defaults.hardware_mapping = "adafruit-hat";
	defaults.pixel_mapper_config = "PiPong";

	rgb_matrix::RuntimeOptions rtoptions;
	rtoptions.gpio_slowdown = 5;

	TheLedCanvas = RGBMatrix::CreateFromFlags(&argc, &args, &defaults, &rtoptions);
	if (TheLedCanvas == NULL)
		return 1;

	OffScreenCanvas = TheLedCanvas->CreateFrameCanvas();

	// It is always good to set up a signal handler to cleanly exit when we
	// receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
	// for that.
	signal(SIGTERM, InterruptHandler);
	signal(SIGINT, InterruptHandler);

	// set the randomness
	srand((unsigned int) time(0));

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
	while (!interrupt_received)
	{
		loop();
		OffScreenCanvas = TheLedCanvas->SwapOnVSync(OffScreenCanvas);
		usleep(1000 * (1000 / GAME_FrameRate));
	}

	// end of application
	DeleteSoundSamples();
  
	// Animation finished. Shut down the RGB matrix.
	TheLedCanvas->Clear();
	delete TheLedCanvas;

	// real end
	return 0;

}

//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//



static void DrawOnCanvas(Canvas *canvas) {
  /*
   * Let's create a simple animation. We use the canvas to draw
   * pixels. We wait between each step to have a slower animation.
   */
  canvas->Fill(0, 0, 255);

  int center_x = canvas->width() / 2;
  int center_y = canvas->height() / 2;
  float radius_max = canvas->width() / 2;
  float angle_step = 1.0 / 360;
  for (float a = 0, r = 0; r < radius_max; a += angle_step, r += angle_step) {
    if (interrupt_received)
      return;
    float dot_x = cos(a * 2 * M_PI) * r;
    float dot_y = sin(a * 2 * M_PI) * r;
    canvas->SetPixel(center_x + dot_x, center_y + dot_y, 255, 255, 255);
    usleep(1 * 1000); // wait a little to slow down things.
  }
}


