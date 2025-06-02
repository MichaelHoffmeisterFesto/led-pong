#pragma once

#include "VectorArith.h"
#include "LedTexture.h"
#include "TextRendererAbstractBase.h"

using namespace std;

struct ProportionalTextGlyph
{
    char Letter = '\0';
    int OfsX = 0;
    int Width = 0;
};

class TextRendererProportionalText :
    public TextRendererAbstractBase
{
public:
    TextRendererProportionalText();
    TextRendererProportionalText(string fn, int numGlyphs, ProportionalTextGlyph* glyphDefs);

    // virtual methods which are implemented need to be repeated
    virtual void DrawTextTo(LedTexture& texture, Vec2 pixelPos,
        const char nullTerminatedText[], int renderSpacingX = 0, int renderSpacingY = 0,
        int thresholdIntensity = -1, int fixedSpacingX = -1, LedColor* colorMultiply = nullptr);

private:
    LedTexture mCharDefs;
    int mNumGlyphs = 0;
    ProportionalTextGlyph* mGlyphDefs = nullptr;
};

