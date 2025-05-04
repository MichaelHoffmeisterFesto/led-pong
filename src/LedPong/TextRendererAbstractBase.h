#pragma once

#include "VectorArith.h"
#include "LedTexture.h"

// This class is the abstract base class for "text renders".
// These are classes with the ability to render single patterns
// of LED pixels to a LedTexture, just identified by an array
// of (char) indices.
class TextRendererAbstractBase
{
public:
	virtual void DrawTextTo(LedTexture& texture, Vec2 pixelPos,
		const char nullTerminatedText[], int renderSpacingX = 0, int renderSpacingY = 0,
		int thresholdIntensity = -1, int fixedSpacingX = -1) = 0;
};

