#include "pch.h"
#include "Pallete.h"

Pallete::Pallete(Image* pImage, float x, float y, int frameX, int frameY, float sizeX, float sizeY)	
	: mImage(pImage), mX(x), mY(y), mFrameX(frameX), mFrameY(frameY), mSizeX(sizeX), mSizeY(sizeY) 
{
	mTileType = TileType::Normal;
	mRect = RectMake(mX, mY, mSizeX, mSizeY);
}

void Pallete::Render(HDC hdc)
{
	mImage->ScaleFrameRender(hdc, mX, mY, mFrameX,mFrameY, mSizeX, mSizeY);

}

void Pallete::Render(HDC hdc, float startX, float startY)
{
	CAMERA->ScaleFrameRender(hdc, mImage, startX, startY, mFrameX, mFrameY, mSizeX, mSizeY);
	//mImage->ScaleFrameRender(hdc, startX, startY, mFrameX, mFrameY, mSizeX, mSizeY);
}

