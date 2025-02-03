#include "VectorArith.h"
#include "TextRendererFixedSize.h"

TextRendererFixedSize::TextRendererFixedSize()
{
	mCharDefs = LedTexture();
	mCharWidth = 0;
	mCharSpacing = 0;
}

TextRendererFixedSize::TextRendererFixedSize(string fn, int charWidth, int charSpacing, string indexToChar)
{
	mCharDefs = LedTexture(fn);
	mCharWidth = charWidth;
	mCharSpacing = charSpacing;
	mIndexToChar = indexToChar;
}

void TextRendererFixedSize::DrawTextTo(
	LedTexture& texture, 
	Vec2 pixelPos,
	const char nullTerminatedText[],
	int renderSpacingX, int renderSpacingY)
{
	// check, if all required definitions are present
	if (mCharWidth < 1
		|| !mCharDefs.HasContent()
		|| !texture.HasContent()
		|| strlen(nullTerminatedText) < 1)
	{
		return;
	}

	// working coordinates
	Vec2 workPos = pixelPos;

	// over all chars
	for (int ci = 0; ci < (int) strlen(nullTerminatedText); ci++)
	{
		// char to search
		char cSearch = nullTerminatedText[ci];

		// new line?
		if (cSearch == '\n')
		{
			workPos.X = pixelPos.X;
			workPos.Y += mCharDefs.GetHeight() + renderSpacingY;
			continue;
		}

		// find index?
		int fi = -1;
		for (int fii=0; fii < (int) mIndexToChar.length(); fii++)
			if (mIndexToChar[fii] == cSearch)
			{
				fi = fii;
				break;
			}
		if (fi < 0)
			continue;

		// blit
		texture.BlitFrom(workPos.X, workPos.Y,			// to X/Y
			mCharDefs,									// source texture
			(mCharWidth + mCharSpacing) * fi, 0,		// srcX/Y
			mCharWidth, -1);							// srcWidth/heigh

		// advance
		workPos.X += mCharWidth + renderSpacingX;
	}
}