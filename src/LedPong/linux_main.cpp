// HKA - Technische Informatik 2
// LedPong - Linux Hauptprogramm
// (c) Michael Hoffmeister

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

#include <termios.h>
#include <fcntl.h>

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
	SoundSamples[GameSoundSampleEnum::SMP_EmptyTile] = new SDL_SoundSample("/home/pi/newpi3/led-pong/src/LedPong/media/pop-268648-shortened-fade-out.wav", MIX_MAX_VOLUME / 4);
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

void DebugSound0()
{
	printf("Music Error %s", SDL_GetError());

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
            cout << "E" << endl;
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

    // this currently interferes with arduino!
    if (false)
    {
        bool keyAny = false;
        for (int i = 1; i < (int) SIZE_OF_ARR(SdlScanToMap); i++)
        {
            TheEnv.GameKey[i] = keys[SdlScanToMap[i]];
            keyAny = keyAny || TheEnv.GameKey[i];
        }
        TheEnv.GameKey[KEY_ANY] = keyAny;
    }

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
   			OffScreenCanvas->SetPixel(2 * 64 - 1 - x, 3*32 - 1 - y, lp->R, lp->G, lp->B );
		}
	}

	//
	// End of loop
	//

	return true;
}

int fd_serialport;
struct termios options;

int init_arduino()
{
    fd_serialport = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);

    if(fd_serialport == -1){
        perror("Unable to open /dev/ttyACM0");
    }

    tcgetattr(fd_serialport, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag |= (CLOCAL | CREAD);
    // options.c_cflag |= PARENB;
    // options.c_cflag |= PARODD;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_iflag |= (INPCK | ISTRIP);
    tcsetattr(fd_serialport, TCSANOW, &options);

    fcntl(fd_serialport, F_SETFL, FNDELAY);

    return 0;
}

char data_in[256] = "\0";
string line_buffer = "";
string line_ready = "";

int hexchars_to_int(const char* chs)
{
    int sum = 0 ;
    for (int i=0; i<4; i++)
    {
        char c = tolower(chs[i]);
        if (c >= '0' && c <= '9')
        {
            sum = 16 * sum + c - '0';
        }
        else
        if (c >= 'a' && c <= 'f')
        {
            sum = 16 * sum + (10 + c - 'a');
        }
        else
            return -1;
    }
    return sum;
}

bool process_arduino_keys(int keybits)
{

    TheEnv.GameKey[KEY_P1_LEFT]  = (keybits & 0x0040) > 0;
    TheEnv.GameKey[KEY_P1_UP]    = (keybits & 0x0004) > 0;
    TheEnv.GameKey[KEY_P1_DOWN]  = (keybits & 0x0020) > 0;
    TheEnv.GameKey[KEY_P1_RIGHT] = (keybits & 0x0010) > 0;
    
    TheEnv.GameKey[KEY_P2_LEFT]  = (keybits & 0x0001) > 0;
    TheEnv.GameKey[KEY_P2_UP]    = (keybits & 0x0200) > 0;
    TheEnv.GameKey[KEY_P2_DOWN]  = (keybits & 0x0080) > 0;
    TheEnv.GameKey[KEY_P2_RIGHT] = (keybits & 0x0100) > 0;
    
    TheEnv.GameKey[KEY_ANY]      = (keybits & 0x7fff) > 0;

    return true;
}

bool check_arduino()
{
    int rdnum = read(fd_serialport, &data_in[0], sizeof(data_in) - 2);
    if (rdnum > 0)
    {
        // printf("%d\n", rdnum);
        // data_in[rdnum] = '\0';
        // printf("%s\n",&data_in[0]);
        for (int i=0; i<rdnum; i++)
        {
            // just add chars?
            char c = data_in[i];
            if (c != '\r' && c != '\n')
            {
                line_buffer += c;
                continue;
            }

            // buffered line ready
            line_ready = line_buffer;
            line_buffer = "";

            // only process well-formed lines
            if (line_ready.length() != 14)
                continue;
            // cout << "|" << line_ready << "|" << endl;

            // apply "hex to int" manually
            const char *cs1 = line_ready.c_str();
            for (int k=0; k<3; k++)
            {
                int n = hexchars_to_int(cs1 + k*5);
                if (n < 0)
                    break;
                // cout << n << endl;
                if (k == 0)
                {
                    // successfully found keys!
                    process_arduino_keys(n);
                }
            }
        }
    }
    return true;
}

int main(int argc, char** args)
{
    // init SLD
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Could not init SDL:" << SDL_GetError() << '\n';
    }
    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        std::cerr << "Could not init SDL:" << SDL_GetError() << '\n';
    }

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
	// receive a CTRL-C for instance. 
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
    init_arduino();
	while (!interrupt_received && check_arduino())
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
}
