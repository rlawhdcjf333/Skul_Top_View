#include "pch.h"
#include "TileList.h"
#include "Tile.h"


bool TileList::MapAreaCheck(float a, float b)
{
	int x = a / TileSizeY + b / TileSizeX - (StartX / TileSizeX + StartY / TileSizeY);
	int y = a / TileSizeY - b / TileSizeX + (StartX / TileSizeX - StartY / TileSizeY);
	int offsetX = (LONG)a % TileSizeX;
	int offsetY = (LONG)b % TileSizeY;

	if (offsetY < TileSizeY / 2 - offsetX / 2) { x--; }
	if (offsetY < offsetX / 2 - TileSizeY / 2) { y--; }
	if (offsetY > offsetX / 2 + TileSizeY / 2) { y++; }
	if (offsetY > 3 * TileSizeY / 2 - offsetX / 2) { x++; }

	if (x >= 0 and x < TILESizeX and y >= 0 and y < TILESizeY)
	{
		return true;
	}
	return false;
}

bool TileList::MapAreaCheck(LONG a, LONG b)
{
	int x = a / TileSizeY + b / TileSizeX - (StartX / TileSizeX + StartY / TileSizeY);
	int y = a / TileSizeY - b / TileSizeX + (StartX / TileSizeX - StartY / TileSizeY);
	int offsetX = a % TileSizeX;
	int offsetY = b % TileSizeY;

	if (offsetY < TileSizeY / 2 - offsetX / 2) { x--; }
	if (offsetY < offsetX / 2 - TileSizeY / 2) { y--; }
	if (offsetY > offsetX / 2 + TileSizeY / 2) { y++; }
	if (offsetY > 3 * TileSizeY / 2 - offsetX / 2) { x++; }

	if (x >= 0 and x < TILESizeX and y >= 0 and y < TILESizeY)
	{
		return true;
	}
	return false;
}
