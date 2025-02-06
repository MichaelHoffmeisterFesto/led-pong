#include "GameConst.h"
#include "GameEnvironment.h"

GameEnvironment::GameEnvironment()
{
	// screen
	Screen = LedTexture(WALL_Xdim, WALL_Ydim);

	// renderers
	TrFs = TextRendererFixedSize(
		"media/pacman-kacheln5x5.bmp", 5, 1,
		"QWERTZUIABCD.-|HJKLtb=lr#jhokasdf uvwx*ceg$§");

	ProportionalTextGlyph glyphs[] = {
		{ 'A', 0, 5 },
		{ 'B', 6, 5 },
		{ 'C', 12, 5 },
		{ 'D', 18, 5 },
		{ 'E', 24, 5 },
		{ 'F', 30, 5 },
		{ 'G', 36, 5 },
		{ 'H', 42, 5 },
		{ 'I', 48, 5 },
		{ 'J', 54, 4 },
		{ 'K', 59, 4 },
		{ 'L', 64, 4 },
		{ 'M', 69, 5 },
		{ 'N', 75, 5 },
		{ 'O', 81, 5 },
		{ 'P', 87, 5 },
		{ 'Q', 93, 5 },
		{ 'R', 99, 5 },
		{ 'S', 105, 5 },
		{ 'T', 111, 5 },
		{ 'U', 117, 5 },
		{ 'V', 123, 5 },
		{ 'W', 129, 5 },
		{ 'X', 135, 5 },
		{ 'Y', 141, 5 },
		{ 'Z', 147, 5 },
		{ '0', 153, 4 },
		{ '1', 158, 4 },
		{ '2', 163, 4 },
		{ '3', 168, 4 },
		{ '4', 173, 4 },
		{ '5', 178, 4 },
		{ '6', 183, 4 },
		{ '7', 188, 4 },
		{ '8', 193, 4 },
		{ '9', 198, 4 },
		{ '-', 203, 3 },
		{ '_', 207, 3 },
		{ '.', 211, 3 },
		{ ',', 214, 4 },
		{ ' ', 218, 3 }
	};

	TrPt = TextRendererProportionalText("media/charset-prop-5x5.bmp",
		SIZE_OF_ARR(glyphs), glyphs);
}

GameEnvironment::~GameEnvironment()
{
}