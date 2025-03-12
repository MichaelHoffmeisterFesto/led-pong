#pragma once
#include "LedTexture.h"

enum LenGenTextureKind { 
    LGT_None, LGT_Plasma,
    LGT_NUM };

class LedGenerativeTexture :
    public LedTexture
{
public:
    // constrained constructors
    ~LedGenerativeTexture();
    LedGenerativeTexture(int width, int height, LenGenTextureKind kind);

private:
    // members
    LenGenTextureKind mKind = LGT_None;
};

