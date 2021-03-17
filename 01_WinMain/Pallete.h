#pragma once
#include "Tile.h"

class Pallete
{
	Image* mImage;
	
	float mX;
	float mY;
	float mSizeX;
	float mSizeY;
	int mFrameX;
	int mFrameY;

	RECT mRect;
	TileType mTileType;
public:
	Pallete(Image* pImage, float x, float y, int frameX, int frameY, float sizeX, float sizeY);
	void Render(HDC hdc);
	void Render(HDC hdc, float startX, float startY);

	Image* GetImage() { return mImage; }
	RECT* GetRect() { return &mRect; }
	int GetFrameX() { return mFrameX; }
	int GetFrameY() { return mFrameY; }

	void SetType(TileType val) { mTileType = val; }
	TileType GetType() { return mTileType; }
};
