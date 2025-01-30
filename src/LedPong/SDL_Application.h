#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

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

