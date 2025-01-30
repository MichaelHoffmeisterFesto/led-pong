#pragma once

#include "Tiles.h"

// This class implements (via a static method) a so called
// "factory", that is, a generator of (different) objects/ class
// instances depending on the factory arguments (in this case: "code").
// Note: This looks a bit artificial, but it is good point for future
// extensibility.
class TileFactory
{
public:
	static TileBase* GenerateTile(char code);
};

