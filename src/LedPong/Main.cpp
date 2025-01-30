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

#include "basic.h"

#include "LedColor.h"
#include "LedColors.h"
#include "LedTexture.h"

#include "TextRendererFixedSize.h"
#include "TextRendererProportionalText.h"

#include "LedAnimation.h"

#include "CQueueDynGeneric.h"

#include "SampleGen.h"

#include "Tiles.h"
#include "TileMap.h"

// provide instances for extern symbols
LedColorsTable LedColors;

using namespace std;

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

// dimensions

// 96:128 = 3:4
const int WALL_Xdim = 3 * 32; 
const int WALL_Ydim = 2 * 64; 

const int WALL_Xgap = 1;
const int WALL_Ygap = 1;

const int WALL_XpixSize = 4;
const int WALL_YpixSize = 4;

//
// STATE
//

LedTexture PageBallons("media\\ballons.bmp");
LedTexture PageFunnyCar("media\\ai_funny_car.bmp");
LedTexture PageInfo2("media\\HKA_Info2.bmp");

CQueueDynGeneric<LedAnimation> AnimationQueue;

int frameCounter = 0;

//struct LedPixelOld
//{
//	uint8_t R, G, B;
//};

// LedColor Pixels[WALL_Xdim][WALL_Ydim];

LedTexture Screen(WALL_Xdim, WALL_Ydim);

double grad = 0.0;

TileMap* level;
TextRendererFixedSize* textRendererFS;
Player player1;

int openMouthTime = 0;

//
// Animation content
//

void AddWelcomeAnimations()
{
	LedAnimation la1;
	CElementGeneric<LedAnimation> TestXYZ(la1);
	TestXYZ.TestAAA();

	AnimationQueue.enqueue(LedAnimation(
		PageInfo2, 1.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect()));

	AnimationQueue.enqueue(LedAnimation(
		PageFunnyCar, 1.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect()));

	AnimationQueue.enqueue(LedAnimation(
		PageBallons, 2.0, LedTexture::GetRandomGradient(), LedTexture::GetRandomBlendEffect()));
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
	// Animate
	//

	//for (int x = 0; x < WALL_Xdim; x++)
	//	for (int y = 0; y < WALL_Ydim; y++)
	//	{
	//		Pixels[x][y].R = rand() % 255;
	//		Pixels[x][y].G = rand() % 255;
	//		Pixels[x][y].B = rand() % 255;
	//	}

	//auto blend = Screen.Clone();
	//blend.FillGradient(LedGradient::TopLeftToRightBottom, grad);
	//Screen.BlendFrom(PageInfo2, PageFunnyCar, blend, LedBlendEffect::Sparcle);

	//grad = grad + 0.02;
	//if (grad > 1.0)
	//	grad = 0.0;

	// Screen.BlitFrom(0, 0, PageFunnyCar);

	double step = 0.01;

	auto ta = AnimationQueue.top();
	if (false && ta != nullptr)
	{
		if (ta->mPhase == 0.0)
		{
			// start of animation
			ta->mStartTexture = Screen.Clone();
		}

		if (ta->mPhase < 1.0)
		{
			// only step in animation
			auto blend = Screen.Clone();
			blend.FillGradient(ta->mGradient, ta->mPhase);
			Screen.BlendFrom(ta->mStartTexture, ta->mTargetTexture, blend, ta->mBlendEffect);

			// go (last) step further ..
			ta->mPhase = std::min(1.0, ta->mPhase + ta->mSpeed * step);
		}
		else
		{
			// animation ended
			Screen.BlitFrom(0, 0, ta->mTargetTexture);

			// remove this animation
			LedAnimation tmp;
			AnimationQueue.dequeue(tmp);
		}
	}

	// restart again?
	if (false && AnimationQueue.GetLength() < 1)
	{
		AddWelcomeAnimations();
	}

	// test
	if (true)
	{
		level->DrawMap(Screen, Vec2(0,0), *textRendererFS, 5, 5);

		bool openMouth = false;
		if (openMouthTime > 0)
		{
			openMouth = (frameCounter % 6) >= 3;
			openMouthTime--;
		}

		char xx[] = { player1.GetPlayerPhase(player1.CurrentDirection, openMouth), '\0' };
		textRendererFS->DrawTextTo(
			Screen, player1.GetCurrentPixelPos(Vec2(5,5)), 
			xx
		);

		// already moved?
		bool alreadyMoved = false;

		// Translate the keyboard into an direction in a way, that a keypress WITH a 
		// possible direction wins
		Vec2 tmpDirs[] = { Vec2(-1,0), Vec2(+1,0), Vec2(0,-1), Vec2(0,+1) };
		SDL_Scancode tmpScans[] = { SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN };
		
		for (int di = 0; di < SIZE_OF_ARR(tmpDirs); di++)
		{
			PossibleMove pm;
			if (level->FindPossibleMoveInDir(&player1, player1.CurrentTilePosition, tmpDirs[di], pm)
				&& player1.StandStill()
				&& keys[tmpScans[di]])
			{
				// initiate new movement
				if (pm.JumpMove)
				{
					// jump to new final position
					player1.CurrentTilePosition = pm.FinalPosition;
				}
				else
				{
					player1.CurrentDirection = tmpDirs[di];
					player1.Animate();
				}

				// no automatic move anymore
				alreadyMoved = true;

				// allow open mouth
				openMouthTime = 6;

				// no more direction
				break;
			}
		}

		// not already moved
		if (!alreadyMoved && !player1.StandStill())
			player1.Animate();
	}

	//
	// Display
	//

	for (int x = 0; x < WALL_Xdim; x++)
	{
		for (int y = 0; y < WALL_Ydim; y++)
		{
			// no object allocation, please!
			LedColor* lp = &Screen[y][x];
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
	sprintf_s(buffer, "Frame %02d", frameCounter);
	SDL_RenderDrawText(Main->Renderer, Main->FontNormal, SDL_Color_From(LedColors.White), spX, spY, buffer);
	frameCounter = (frameCounter + 1) % 60;

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

	//LedTexture lt1("Test1_24.bmp");
	//Screen.BlitFrom(6, 6, lt1);

	//LedTexture lt2("media\\pacman-kacheln5x5.bmp");
	//Screen.BlitFrom(0, 0, lt2);

	TextRendererFixedSize trfs1(
		"media/pacman-kacheln5x5.bmp", 5, 1,
		"QWERTZUIABCD.-|HJKLtb=lr#jhokasdf uvwx");
	textRendererFS = &trfs1;

	//trfs1.DrawTextTo(Screen, 0, 0, "L----H\n|QWER|\nK----J");

	//ProportionalTextGlyph glyphs[] = {
	//	{ 'A', 0, 5 },
	//	{ 'B', 6, 5 },
	//	{ 'C', 12, 5 },
	//	{ 'D', 18, 5 },
	//	{ 'E', 24, 5 },
	//	{ 'F', 30, 5 },
	//	{ 'G', 36, 5 },
	//	{ 'H', 42, 5 },
	//	{ 'I', 48, 5 },
	//	{ 'J', 54, 4 },
	//	{ 'K', 59, 4 },
	//	{ 'L', 64, 4 },
	//	{ 'M', 69, 5 },
	//	{ 'N', 75, 5 },
	//	{ 'O', 81, 5 },
	//	{ 'P', 87, 5 },
	//	{ 'Q', 93, 5 },
	//	{ 'R', 99, 5 },
	//	{ 'S', 105, 5 },
	//	{ 'T', 111, 5 },
	//	{ 'U', 117, 5 },
	//	{ 'V', 123, 5 },
	//	{ 'W', 129, 5 },
	//	{ 'X', 135, 5 },
	//	{ 'Y', 141, 5 },
	//	{ 'Z', 147, 5 },
	//	{ '0', 153, 4 },
	//	{ '1', 158, 4 },
	//	{ '2', 163, 4 },
	//	{ '3', 168, 4 },
	//	{ '4', 173, 4 },
	//	{ '5', 178, 4 },
	//	{ '6', 183, 4 },
	//	{ '7', 188, 4 },
	//	{ '8', 193, 4 },
	//	{ '9', 198, 4 },
	//	{ '-', 203, 3 },
	//	{ '_', 207, 3 },
	//	{ '.', 211, 3 },
	//	{ ',', 214, 4 },
	//	{ ' ', 218, 3 }
	//};

	//TextRendererProportionalText trpt1("media/charset-prop-5x5.bmp", 
	//	SIZE_OF_ARR(glyphs), glyphs);

	//trpt1.DrawTextTo(Screen, 0, 40, "ABCDEFGHIJK\nLMNOPQRSTUV\nWX_Y-Z\n9876543210,0.X", 1, 1);

	//for (int y=0; y<WALL_Ydim; y++)
	//	for (int x = 0; x < WALL_Xdim; x++)
	//	{
	//		// double f = std::max(0.0, std::min(1.0, 0.1 * (1.0 * x - 48.0)));
	//		// double f = std::max(0.0, std::min(1.0, 0.1 * (1.0 * y - 64.0)));

	//		double f = std::max(0.0, std::min(1.0, 
	//					0.05 * ((1.0 * y - 64.0) + (1.0 * x - 18.0))
	//			));

	//		Screen.Put(x, y, LedColors.White * f);
	//	}

	AddWelcomeAnimations();

	string x1[] = {
		"L-----------------H",
		"|.................|",
		"|.LH.L-H.L------H.|",
		"|.||.| |.|      |.|",
		"|.KJ.K-J.K------J.|",
		"|.................|",
		"|.LH.L-------H.LH.|",
		"|.KJ.K-------J.KJ.|",
		"|.................|",
		"K--H.L--H.L--H.L--J",
		"   |.| LJ.KH |.|   ",
		"---J.| |...| |.K---",
		".....| |...| |.....",
		"---H.| |...| |.L---",
		"   |.| KH.LJ |.|   ",
		"L--J.K--J.K--J.K--H",
		"|.................|",
		"|.LH.L--H.L--H.LH.|",
		"|.||.u--J.u--J.||.|",
		"|.KJ.|....|....||.|",
		"|....|..--J.L--J|.|",
		"|.LH.|......|   |.|",
		"|.KJ.K------w---J.|",
		"|.................|",
		"K-----------------J"
	};
	TileMap tm1(SIZE_OF_ARR(x1), x1);
	tm1.GateToLeft = Vec2(0, 12);
	tm1.GateToRight = Vec2(18, 12);

	string x2[] = {
		"L--------H|.|L----H",
		"|........||.||....|",
		"|.L----H.KJ.KJ.LH.|",
		"|.|    |.......KJ.|",
		"|.K----J.L-H......|",
		"|........K-J.L--H.|",
		"|.L--H.......|  |.|",
		"|.K--w-------w--J.|",
		"|.................|",
		"K--H.L--H.L--H.L--J",
		"   |.| LJ.KH |.|   ",
		"---J.| |...| |.K---",
		".....| |...| |.....",
		"---H.| |...| |.L---",
		"   |.| KH.LJ |.|   ",
		"L--J.K--J.K--J.K--H",
		"|.................|",
		"|.LH.LH.L-x----xH.|",
		"|.||.||.K-J....||.|",
		"|.||.||.....LH.||.|",
		"|.|| Kw-----wJ.KJ.|",
		"|.||..............|",
		"|.KJ.LH.LH.LH.L-H.|",
		"|....||.||.||.| |.|",
		"K----J|.|K-ww-w-w-J"
	};
	TileMap tm2(SIZE_OF_ARR(x2), x2);

	level = &tm1;
	tm1.DrawMap(Screen, Vec2(0, 0), trfs1, 5, 5);
	player1.CurrentTilePosition = Vec2(9, 21);

	Main = new SDL_Application();

	if (!Main->init(800, 670)) return 1;

	while (loop()) {
		// wait before processing the next frame
		SDL_Delay(1000 / 30);
	}

	Main->kill();
	return 0;
}