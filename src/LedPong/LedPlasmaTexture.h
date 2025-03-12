#pragma once
#include "LedTexture.h"
class LedPlasmaTexture :
    public LedTexture
{
public:
    // constrained set of constructors
    LedPlasmaTexture(int width, int height);
    ~LedPlasmaTexture();

    // will be called by the constructor
    void Init(); 

    // needs to be called by the user application
    void Loop();

private:
    // take the originally named variables
    // see: https://github.com/hzeller/rpi-rgb-led-matrix/issues/177
    float d1 = 0, d2 = 0;
    int dx = 0, dy = 0;
    float hueShift = 0;
};

