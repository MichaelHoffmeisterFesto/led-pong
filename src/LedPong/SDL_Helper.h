#pragma once

#ifdef Use_SDL_Grafix

#include <iostream>

#ifdef WIN32
#include <SDL.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#include "LedColor.h"

void SDL_SetRenderDrawColor(SDL_Renderer* Renderer, const SDL_Color* col);
int SDL_RenderDrawCircle(SDL_Renderer* Renderer, int x, int y, int radius);
int SDL_RenderFillCircle(SDL_Renderer* Renderer, int x, int y, int radius);
void SDL_RenderDrawText(SDL_Renderer* Renderer, TTF_Font* font, SDL_Color fg, int x, int y, std::string text);
SDL_Point* TranslatePoints(SDL_Point* points, int num, double preX, double preY, double scale, double angle, int postX, int postY);
SDL_Color SDL_Color_From(LedColor lc);

#endif