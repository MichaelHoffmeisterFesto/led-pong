#include <iostream>
#include "SDL_Helper.h"
#include "LedColor.h"

using namespace std;

int
SDL_RenderDrawCircle(SDL_Renderer* Renderer, int x, int y, int radius)
{
	int offsetx, offsety, d;
	int status;

	// CHECK_RENDERER_MAGIC(renderer, -1);

	offsetx = 0;
	offsety = radius;
	d = radius - 1;
	status = 0;

	while (offsety >= offsetx) {
		status += SDL_RenderDrawPoint(Renderer, x + offsetx, y + offsety);
		status += SDL_RenderDrawPoint(Renderer, x + offsety, y + offsetx);
		status += SDL_RenderDrawPoint(Renderer, x - offsetx, y + offsety);
		status += SDL_RenderDrawPoint(Renderer, x - offsety, y + offsetx);
		status += SDL_RenderDrawPoint(Renderer, x + offsetx, y - offsety);
		status += SDL_RenderDrawPoint(Renderer, x + offsety, y - offsetx);
		status += SDL_RenderDrawPoint(Renderer, x - offsetx, y - offsety);
		status += SDL_RenderDrawPoint(Renderer, x - offsety, y - offsetx);

		if (status < 0) {
			status = -1;
			break;
		}

		if (d >= 2 * offsetx) {
			d -= 2 * offsetx + 1;
			offsetx += 1;
		}
		else if (d < 2 * (radius - offsety)) {
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else {
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}

	return status;
}

int
SDL_RenderFillCircle(SDL_Renderer* Renderer, int x, int y, int radius)
{
	int offsetx, offsety, d;
	int status;

	// CHECK_RENDERER_MAGIC(renderer, -1);

	offsetx = 0;
	offsety = radius;
	d = radius - 1;
	status = 0;

	while (offsety >= offsetx) {

		status += SDL_RenderDrawLine(Renderer, x - offsety, y + offsetx,
			x + offsety, y + offsetx);
		status += SDL_RenderDrawLine(Renderer, x - offsetx, y + offsety,
			x + offsetx, y + offsety);
		status += SDL_RenderDrawLine(Renderer, x - offsetx, y - offsety,
			x + offsetx, y - offsety);
		status += SDL_RenderDrawLine(Renderer, x - offsety, y - offsetx,
			x + offsety, y - offsetx);

		if (status < 0) {
			status = -1;
			break;
		}

		if (d >= 2 * offsetx) {
			d -= 2 * offsetx + 1;
			offsetx += 1;
		}
		else if (d < 2 * (radius - offsety)) {
			d += 2 * offsety - 1;
			offsety -= 1;
		}
		else {
			d += 2 * (offsety - offsetx - 1);
			offsety -= 1;
			offsetx += 1;
		}
	}

	return status;
}

void
SDL_SetRenderDrawColor(SDL_Renderer* Renderer, const SDL_Color* col)
{
	SDL_SetRenderDrawColor(Renderer, col->r, col->g, col->b, col->a);
}

SDL_Point* TranslatePoints(SDL_Point* points, int num, double preX, double preY, double scale, double angle, int postX, int postY)
{
	double phi = angle / 360.0 * 2 * 3.1415;
	SDL_Point* res = new SDL_Point[num];
	for (int i = 0; i < num; i++)
	{
		double x = scale * (preX + points[i].x);
		double y = scale * (preY + points[i].y);

		double rx = x * cos(phi) - y * sin(phi);
		double ry = x * sin(phi) + y * cos(phi);

		res[i].x = postX + (int)rx;
		res[i].y = postY + (int)ry;
	}
	return res;
}

// ----------------------- True Type Font -----------------------------------

void
SDL_RenderDrawText(SDL_Renderer* Renderer, TTF_Font* font, SDL_Color fg, int x, int y, string text)
{
	SDL_Surface* text_surf = TTF_RenderText_Blended(font, text.c_str(), fg);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, text_surf);

	SDL_Rect dest;
	dest.x = x; // - (text_surf->w / 2.0f);
	dest.y = y;
	dest.w = text_surf->w;
	dest.h = text_surf->h;
	SDL_RenderCopy(Renderer, texture, nullptr, &dest);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(text_surf);
}

// --------------------------- Color conversion ---------------------------
SDL_Color SDL_Color_From(LedColor lc)
{
	return SDL_Color{ lc.R, lc.G, lc.B };
}