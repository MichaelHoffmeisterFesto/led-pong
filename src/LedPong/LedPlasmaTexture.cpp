#include "LedPlasmaTexture.h"

#include <algorithm>
#include <math.h>

LedPlasmaTexture::LedPlasmaTexture(int width, int height)
	: LedTexture(width, height)
{
	Init();
}

LedPlasmaTexture::~LedPlasmaTexture()
{
}

// see: https://github.com/hzeller/rpi-rgb-led-matrix/issues/177

void LedPlasmaTexture::Init()
{
	d1 = sqrt(std::max(mWidth, mHeight)) * 1.5;
	d2 = d1 / 2.0;

	int dx = rand() % 1024;
	int dy = rand() % 1024;
}

struct HSVColor {
    HSVColor(uint8_t h_, uint8_t s_, uint8_t v_) : h(h_), s(s_), v(v_) {}
    uint8_t h;
    uint8_t s;
    uint8_t v;
};

LedColor HsvToRGB(HSVColor hsv)
{
    LedColor rgb(hsv.v, hsv.v, hsv.v); // Default to case where hsv.s == 0
    unsigned char region, remainder, p, q, t;

    if (hsv.s == 0) return rgb;

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6;

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.R = hsv.v; rgb.G = t; rgb.B = p;
            break;
        case 1:
            rgb.R = q; rgb.G = hsv.v; rgb.B = p;
            break;
        case 2:
            rgb.R = p; rgb.G = hsv.v; rgb.B = t;
            break;
        case 3:
            rgb.R = p; rgb.G = q; rgb.B = hsv.v;
            break;
        case 4:
            rgb.R = t; rgb.G = p; rgb.B = hsv.v;
            break;
        default:
            rgb.R = hsv.v; rgb.G = p; rgb.B = q;
            break;
    }

    return rgb;
}

void LedPlasmaTexture::Loop()
{
    for (int y = 0; y < mHeight; y++) {
        for (int x = 0; x < mWidth; x++) {
            unsigned int x1 = x + dx;
            unsigned int y1 = y + dy;
            float value = sin(x1 / d1);
            value += sin(y1 / d2);
            value += sin((x1 + y1) / d1);
            value += sin(sqrt(x1 * x1 + y1 * y1) / d2);
            value += 4; // shift range from -4 .. 4 to 0 .. 8
            value /= 8; // bring range down to 0 .. 1

            // value >= 0 && value =< 1.0
            float hue = hueShift + value;
            if (hue >= 1.0) hue -= 1.0;

            // set
            LedColor c = HsvToRGB(HSVColor(hue * 255, 255, 128));
            Put(x, y, c);

            // finally
            dx = (dx + 1) % 1024;
            dy = (dy + 1) % 1024;
            hueShift += 0.02; if (hueShift >= 1.0) hueShift -= 1.0;
        }
    }
}