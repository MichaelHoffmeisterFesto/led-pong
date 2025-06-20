#pragma once

#include <stdint.h> // for standard integer types like uint8_t ..
#include <algorithm> // max, min

// Class definition of a RGB-tuple for LED display.
// Note: This could also have been realized as struct {},
// mainly for optimization reasons, but for the sake of 
// Object Orientation it is an class.
class LedColor
{
public:
	uint8_t R, G, B;
	
	// Default constructor is black
	LedColor();
	
	// Initialize with RGB values
	LedColor(uint8_t r, uint8_t g, uint8_t b);
	LedColor(uint32_t rgb);

	inline LedColor operator * (double f)
	{
		LedColor res{ 
			(uint8_t) std::max(0, std::min(255, (int) (f * R) )),
			(uint8_t) std::max(0, std::min(255, (int) (f * G) )),
			(uint8_t) std::max(0, std::min(255, (int) (f * B) ))
		};
		return res;
	}

	inline LedColor operator * (LedColor other)
	{
		LedColor res{
			(uint8_t)std::max(0, std::min(255, (int)(other.R) * R / 256)),
			(uint8_t)std::max(0, std::min(255, (int)(other.G) * G / 256)),
			(uint8_t)std::max(0, std::min(255, (int)(other.B) * B / 256))
		};
		return res;
	}

	inline static LedColor FadeBetween (LedColor color1, LedColor color2, double f)
	{
		LedColor res{
			(uint8_t)std::max(0, std::min(255, (int)(f * color2.R + (1.0-f) * color1.R))),
			(uint8_t)std::max(0, std::min(255, (int)(f * color2.G + (1.0-f) * color1.G))),
			(uint8_t)std::max(0, std::min(255, (int)(f * color2.B + (1.0-f) * color1.B)))
		};
		return res;
	}

	inline unsigned char PercievedIntensity()
	{
		double d = std::max(0.0, std::min(255.0,
			(0.21 * R) + (0.72 * G) + (0.07 * B)));
		return (unsigned char)(d);
	}
};

