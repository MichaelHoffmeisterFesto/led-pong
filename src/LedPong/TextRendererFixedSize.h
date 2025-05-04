#pragma once

#include "VectorArith.h"
#include "LedTexture.h"
#include "TextRendererAbstractBase.h"

using namespace std;

class TextRendererFixedSize :
    public TextRendererAbstractBase
{
public:
    TextRendererFixedSize();
    TextRendererFixedSize(string fn, int charWidth, int charSpacing, string mIndexToChar);

    // virtual methods which are implemented need to be repeated
    virtual void DrawTextTo(LedTexture& texture, Vec2 pixelPos,
        const char nullTerminatedText[], int renderSpacingX = 0, int renderSpacingY = 0,
        int thresholdIntensity = -1, int fixedSpacingX = -1);

private:
    LedTexture mCharDefs;
    string mIndexToChar = "";
    int mCharWidth = 0;
    int mCharSpacing = 0;
};

