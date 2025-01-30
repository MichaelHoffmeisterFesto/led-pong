#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string.h>

#include "LedColor.h"

// Different gradients to create blending functions
enum LedGradient { LeftToRight, RightToLeft, TopToBottom, TopLeftToRightBottom, __LedGradient_Count };

// Special effects for the blending frontier
enum LedBlendEffect { No, Sparcle, __LedBlendEffect_Count };

class LedTexture
{
public:
	// constructors
	LedTexture();
	LedTexture(int width, int height);
	LedTexture(int width, int height, LedColor* source, bool doCopy);
	LedTexture(std::string fn);

	// cloning
	LedTexture& Clone();

	// accessors
	inline int GetWidth() { return mWidth; }
	inline int GetHeight() { return mHeight; }

	inline bool HasContent()
	{
		return mPixels != nullptr && mWidth >= 0 && mHeight >= 0;
	}

	// safe, access of single pixles
	LedColor Get(int x, int y);
	void Put(int x, int y, LedColor pixel);

	// unsafe!!
	LedColor* operator[](int row);

	// drawing
	void FillRect(LedColor color, int x = 0, int y = 0, int width = -1, int height = -1);

	// different gradients for blending
	/// <param name='gradient'>Type of gradient function</param>
	/// <param name='f'>Position of gradient from side to side (0.0 .. 1.0)</param>
	void FillGradient(LedGradient gradient, double f);

	// Blitting means copying rectangular fields of pixels
	bool BlitFrom(
		int destX, int destY,
		LedTexture& src,
		int srcX = 0, int srcY = 0,
		int srcWidth = -1, int srcHeight = -1);

	// Blending means, that source1 and source2 are blended pixel by pixel
	// depending on the respective pixel of the gradient.
	// Note: all textures have to have the same dimensions, for simplicity.
	bool BlendFrom(
		LedTexture& source1,
		LedTexture& source2,
		LedTexture& gradient,
		LedBlendEffect effect);

	// Randomize
	static LedGradient GetRandomGradient();
	static LedBlendEffect GetRandomBlendEffect();

private:
	int mWidth = 0;
	int mHeight = 0;
	LedColor* mPixels = nullptr;

	// internal functions
	bool LoadBmpInternal(std::string fn);
};

