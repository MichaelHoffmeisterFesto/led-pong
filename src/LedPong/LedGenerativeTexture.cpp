#include "LedGenerativeTexture.h"

LedGenerativeTexture::LedGenerativeTexture(int width, int height, LenGenTextureKind kind) 
	: LedTexture(width, height), mKind(kind)
{
}

LedGenerativeTexture::~LedGenerativeTexture()
{
}

