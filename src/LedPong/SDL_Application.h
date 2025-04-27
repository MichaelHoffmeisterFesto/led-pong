#pragma once

#ifdef Use_SDL_Grafix

#ifdef WIN32
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#else
#endif

class SDL_Application
{
public:
	SDL_Window* Window = nullptr;
	SDL_Renderer* Renderer = nullptr;
	TTF_Font* FontNormal = nullptr;
	SDL_Surface* ImageHka = nullptr;

	int Width = 0;
	int Height = 0;

	bool init(int width, int height);
	void kill();	
};

#endif
