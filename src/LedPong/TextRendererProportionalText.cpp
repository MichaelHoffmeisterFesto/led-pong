#include "TextRendererProportionalText.h"

TextRendererProportionalText::TextRendererProportionalText(
	string fn, 
	int numGlyphs,
	ProportionalTextGlyph* glyphDefs)
{
	mCharDefs = LedTexture(fn);
	mNumGlyphs = numGlyphs;
	mGlyphDefs = glyphDefs;
}

void TextRendererProportionalText::DrawTextTo(
	LedTexture& texture,
	Vec2 pixelPos,
	const char nullTerminatedText[],
	int renderSpacingX, int renderSpacingY)
{
	// check, if all required definitions are present
	if (!mCharDefs.HasContent()
		|| mNumGlyphs < 1
		|| mGlyphDefs == nullptr
		|| !texture.HasContent()
		|| strlen(nullTerminatedText) < 1)
	{
		return;
	}

	// working coordinates
	Vec2 workPos = pixelPos;

	// over all chars
	for (int ci = 0; ci < strlen(nullTerminatedText); ci++)
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
		for (int fii = 0; fii < mNumGlyphs; fii++)
			if (mGlyphDefs[fii].Letter == cSearch)
			{
				fi = fii;
				break;
			}
		if (fi < 0)
			continue;

		// glyph
		ProportionalTextGlyph glyph = mGlyphDefs[fi];

		// blit
		texture.BlitFrom(workPos.X, workPos.Y,			// to X/Y
			mCharDefs,									// source texture
			glyph.OfsX, 0,								// srcX/Y
			glyph.Width, -1);							// srcWidth/heigh

		// advance
		workPos.X += glyph.Width + renderSpacingX;
	}
}