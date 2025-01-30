#pragma once

#include "LedColor.h"

// Note:
// It is not easy to member-initialize a static class instance within
// a class definition. Therefore provide a table of "normal" class instances
// and make it a singleton via "extern"

class LedColorsTable
{
public:
	// generic
	LedColor Black{ 0x000000 };
	LedColor White{ 0xffffff };
	LedColor Red{ 0xff0000 };
	LedColor Green{ 0x00ff00 };
	LedColor Blue{ 0x0000ff };

	// see: https://www.reddit.com/r/spaceengineers/comments/3e0k38/rgb_values_for_various_types_of_realworld_lights/
	LedColor Tungsten40W{ 255, 197, 143 };
	LedColor Tungsten100W{ 255, 214, 170 };
	LedColor Halogen{ 255, 241, 224 };
	LedColor CarbonArc{ 255, 250, 244 };
	LedColor HighNoonSun{ 255, 255, 251 };
	LedColor DirectSunlight{ 255, 255, 255 };
	LedColor OvercastSky{ 201, 226, 255 };
	LedColor ClearBlueSky{ 64, 156, 255 };
	LedColor WarmFluorescent{ 255, 244, 229 };
	LedColor StandardFluorescent{ 244, 255, 250 };
	LedColor CoolWhiteFluorescent{ 212, 235, 255 };
	LedColor FullSpectrumFluorescent{ 255, 244, 242 };
	LedColor GrowLightFluorescent{ 255, 239, 247 };
	LedColor BlackLightFluorescent{ 167, 0, 255 };
	LedColor MercuryVapor{ 216, 247, 255 };
	LedColor SodiumVapor{ 255, 209, 178 };
	LedColor MetalHalide{ 242, 252, 255 };
	LedColor HighPressureSodium{ 255, 183, 76 };

	// see: https://www.rapidtables.com/web/color/RGB_Color.html
	LedColor Lime{ 0x00ff00 };
	LedColor Yellow{ 0xFfff00 };
	LedColor Cyan { 0x00ffff };
	LedColor Magenta { 0xFf00ff };
	LedColor Silver{ 0xC0c0c0 };
	LedColor Gray{ 0x808080 };
	LedColor Olive{ 0x808000 };
	LedColor Middle_Green{ 0x008000 };
	LedColor Purple{ 0x800080 };
	LedColor Teal{ 0x008080 };
	LedColor Navy{ 0x000080 };
	LedColor Maroon{ 0x800000 };
	LedColor Dark_Red{ 0x8b0000 };
	LedColor Brown{ 0xA52a2a };
	LedColor Firebrick{ 0xB22222 };
	LedColor Crimson{ 0xDc143c };
	LedColor Tomato{ 0xFf6347 };
	LedColor Coral{ 0xFf7f50 };
	LedColor Indian_Red{ 0xCd5c5c };
	LedColor Light_Coral{ 0xF08080 };
	LedColor Dark_Salmon{ 0xE9967a };
	LedColor Salmon{ 0xFa8072 };
	LedColor Light_Salmon{ 0xFfa07a };
	LedColor Orange_Red{ 0xFf4500 };
	LedColor Dark_Orange{ 0xFf8c00 };
	LedColor Orange{ 0xFfa500 };
	LedColor Gold{ 0xFfd700 };
	LedColor Dark_Golden_Rod{ 0xB8860b };
	LedColor Golden_Rod{ 0xDaa520 };
	LedColor Pale_Golden_Rod{ 0xEee8aa };
	LedColor Dark_Khaki{ 0xBdb76b };
	LedColor Khaki{ 0xF0e68c };
	LedColor Yellow_Green{ 0x9acd32 };
	LedColor Dark_Olive_Green{ 0x556b2f };
	LedColor Olive_Drab{ 0x6b8e23 };
	LedColor Lawn_Green{ 0x7cfc00 };
	LedColor Chartreuse{ 0x7fff00 };
	LedColor Green_Yellow{ 0xAdff2f };
	LedColor Dark_Green{ 0x006400 };
	LedColor Forest_Green{ 0x228b22 };
	LedColor Lime_Green{ 0x32cd32 };
	LedColor Light_Green{ 0x90ee90 };
	LedColor Pale_Green{ 0x98fb98 };
	LedColor Dark_Sea_Green{ 0x8fbc8f };
	LedColor Medium_Spring_Green{ 0x00fa9a };
	LedColor Spring_Green{ 0x00ff7f };
	LedColor Sea_Green{ 0x2e8b57 };
	LedColor Medium_Aqua_Marine{ 0x66cdaa };
	LedColor Medium_Sea_Green{ 0x3cb371 };
	LedColor Light_Sea_Green{ 0x20b2aa };
	LedColor Dark_Slate_Gray{ 0x2f4f4f };
	LedColor Dark_Cyan{ 0x008b8b };
	LedColor Aqua{ 0x00ffff };
	LedColor Light_Cyan{ 0xE0ffff };
	LedColor Dark_Turquoise{ 0x00ced1 };
	LedColor Turquoise{ 0x40e0d0 };
	LedColor Medium_Turquoise{ 0x48d1cc };
	LedColor Pale_Turquoise{ 0xAfeeee };
	LedColor Aqua_Marine{ 0x7fffd4 };
	LedColor Powder_Blue{ 0xB0e0e6 };
	LedColor Cadet_Blue{ 0x5f9ea0 };
	LedColor Steel_Blue{ 0x4682b4 };
	LedColor Corn_Flower_Blue{ 0x6495ed };
	LedColor Deep_Sky_Blue{ 0x00bfff };
	LedColor Dodger_Blue{ 0x1e90ff };
	LedColor Light_Blue{ 0xAdd8e6 };
	LedColor Sky_Blue{ 0x87ceeb };
	LedColor Light_Sky_Blue{ 0x87cefa };
	LedColor Midnight_Blue{ 0x191970 };
	LedColor Dark_Blue{ 0x00008b };
	LedColor Medium_Blue{ 0x0000cd };
	LedColor Royal_Blue{ 0x4169e1 };
	LedColor Blue_Violet{ 0x8a2be2 };
	LedColor Indigo{ 0x4b0082 };
	LedColor Dark_Slate_Blue{ 0x483d8b };
	LedColor Slate_Blue{ 0x6a5acd };
	LedColor Medium_Slate_Blue{ 0x7b68ee };
	LedColor Medium_Purple{ 0x9370db };
	LedColor Dark_Magenta{ 0x8b008b };
	LedColor Dark_Violet{ 0x9400d3 };
	LedColor Dark_Orchid{ 0x9932cc };
	LedColor Medium_Orchid{ 0xBa55d3 };
	LedColor Thistle{ 0xD8bfd8 };
	LedColor Plum{ 0xDda0dd };
	LedColor Violet{ 0xEe82ee };
	LedColor Fuchsia{ 0xFf00ff };
	LedColor Orchid{ 0xDa70d6 };
	LedColor Medium_Violet_Red{ 0xC71585 };
	LedColor Pale_Violet_Red{ 0xDb7093 };
	LedColor Deep_Pink{ 0xFf1493 };
	LedColor Hot_Pink{ 0xFf69b4 };
	LedColor Light_Pink{ 0xFfb6c1 };
	LedColor Pink{ 0xFfc0cb };
	LedColor Antique_White{ 0xFaebd7 };
	LedColor Beige{ 0xF5f5dc };
	LedColor Bisque{ 0xFfe4c4 };
	LedColor Blanched_Almond{ 0xFfebcd };
	LedColor Wheat{ 0xF5deb3 };
	LedColor Corn_Silk{ 0xFff8dc };
	LedColor Lemon_Chiffon{ 0xFffacd };
	LedColor Light_Golden_Rod_Yellow{ 0xFafad2 };
	LedColor Light_Yellow{ 0xFfffe0 };
	LedColor Saddle_Brown{ 0x8b4513 };
	LedColor Sienna{ 0xA0522d };
	LedColor Chocolate{ 0xD2691e };
	LedColor Peru{ 0xCd853f };
	LedColor Sandy_Brown{ 0xF4a460 };
	LedColor Burly_Wood{ 0xDeb887 };
	LedColor Tan{ 0xD2b48c };
	LedColor Rosy_Brown{ 0xBc8f8f };
	LedColor Moccasin{ 0xFfe4b5 };
	LedColor Navajo_White{ 0xFfdead };
	LedColor Peach_Puff{ 0xFfdab9 };
	LedColor Misty_Rose{ 0xFfe4e1 };
	LedColor Lavender_Blush{ 0xFff0f5 };
	LedColor Linen{ 0xFaf0e6 };
	LedColor Old_Lace{ 0xFdf5e6 };
	LedColor Papaya_Whip{ 0xFfefd5 };
	LedColor Sea_Shell{ 0xFff5ee };
	LedColor Mint_Cream{ 0xF5fffa };
	LedColor Slate_Gray{ 0x708090 };
	LedColor Light_Slate_Gray{ 0x778899 };
	LedColor Light_Steel_Blue{ 0xB0c4de };
	LedColor Lavender{ 0xE6e6fa };
	LedColor Floral_White{ 0xFffaf0 };
	LedColor Alice_Blue{ 0xF0f8ff };
	LedColor Ghost_White{ 0xF8f8ff };
	LedColor Honeydew{ 0xF0fff0 };
	LedColor Ivory{ 0xFffff0 };
	LedColor Azure{ 0xF0ffff };
	LedColor Snow{ 0xFffafa };
	LedColor Dim_Gray { 0x696969 };
	LedColor Dark_Gray { 0xA9a9a9 };
	LedColor Light_Gray { 0xD3d3d3 };
	LedColor Gainsboro{ 0xDcdcdc };
	LedColor White_Smoke{ 0xF5f5f5 };

};

// make "LedColors" a singleton
extern LedColorsTable LedColors;