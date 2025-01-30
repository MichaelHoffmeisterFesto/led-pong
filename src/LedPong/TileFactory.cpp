#include <stdexcept>

#include "TileFactory.h"

TileBase* TileFactory::GenerateTile(char code)
{
	switch (code)
	{
		// Players
		case 'Q':
		case 'W':
		case 'E':
		case 'R':
		case 'T':
		case 'Z':
		case 'U':
		case 'I':
		// Ghosts
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		// Hall, pill
		case '.':
		// Hall, empty
		case ' ':
		// Walls
		case '-':
		case '|':
		case 'H':
		case 'J':
		case 'K':
		case 'L':
		case 't':
		case 'b':
		case '=':
		case 'l':
		case 'r':
		case '#':
		case 'j':
		case 'h':
		case 'o':
		case 'k':
		case 'a':
		case 's':
		case 'd':
		case 'f':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		{
			Tile* res = new Tile;
			res->mTileCode = code;
			return res;
		}

		default:
		{
			throw std::invalid_argument("unknown tile");
		}
	}
}