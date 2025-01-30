#include <iostream>
#include "SDL_Application.h"

using namespace std;

bool SDL_Application::init(int width, int height) {
	// See last example for comments
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	// Initialize SDL_ttf
	if (TTF_Init() < 0) {
		cout << "Error intializing SDL_ttf: " << TTF_GetError() << endl;
		return false;
	}

	this->Width = width;
	this->Height = height;
	Window = SDL_CreateWindow("LED wall Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_RESIZABLE);
	if (!Window) {
		cout << "Error creating window: " << SDL_GetError() << endl;
		system("pause");
		return false;
	}

	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	if (!Renderer) {
		cout << "Error creating renderer: " << SDL_GetError() << endl;
		return false;
	}

	ImageHka = SDL_LoadBMP("media/hka.bmp");
	if (!ImageHka) {
		// load failed
	}

	// Load font
	FontNormal = TTF_OpenFont("media/consola.TTF", 16);
	if (!FontNormal) {
		cout << "Error loading font: " << TTF_GetError() << endl;
		return false;
	}

	// Audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		// Error message if can't initialize
	}

	// Amount of channels (Max amount of sounds playing at the same time)
	Mix_AllocateChannels(32);

	// Rest
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	SDL_RenderClear(Renderer);
	return true;
}

void SDL_Application::kill() {

	// free
	SDL_FreeSurface(ImageHka);

	// Quit
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}