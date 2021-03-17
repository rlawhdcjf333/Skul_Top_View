#include "pch.h"
#include "MapObjectPallete.h"

MapObjectPallete::MapObjectPallete(Image* image, int x, int y)
{
	mImage = image;
	mX = x;
	mY = y;
	mSizeX = 50;
	mSizeY = 25;
	mRect = RectMake(mX, mY, mSizeX, mSizeY);
}

void MapObjectPallete::Render(HDC hdc)
{
	mImage->ScaleRender(hdc, mRect.left, mRect.top, mSizeX, mSizeY);
}

void MapObjectPallete::Render(HDC hdc, LONG x, LONG y)
{
	CAMERA->ScaleRender(hdc,mImage, x, y, mSizeX, mSizeY);
}
