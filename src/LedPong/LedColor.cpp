#include "LedColor.h"

// Default constructor is black
LedColor::LedColor() : R(0), G(0), B(0) 
{
}

// Initialize with RGB values
LedColor::LedColor(uint8_t r, uint8_t g, uint8_t b) : R(r), G(g), B(b)
{
}

LedColor::LedColor(uint32_t rgb)
{
	R = (rgb & 0x00ff0000) >> 16;
	G = (rgb & 0x0000ff00) >> 8;
	B = rgb & 0x000000ff;
}
