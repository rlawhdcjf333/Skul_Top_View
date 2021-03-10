#pragma once
#include "pch.h"
#include "Tile.h"

Tile::Tile(Image* pImage, float x, float y, int frameX, int frameY, int sizeX, int sizeY)
	:mX(x), mY(y), mFrameX(frameX), mFrameY(frameY), mSizeX(sizeX), mSizeY(sizeY), mImage(pImage)
{
	mRect = RectMake(mX, mY, mSizeX, mSizeY);
	mDiam = DiamMake(mX, mY, mSizeX, mSizeY);
}


void Tile::Render(HDC hdc)
{

	//Gizmo::GetInstance()->DrawDiam(hdc, mDiam, Gizmo::Color::Green);
	if (mImage)
	{
		mImage->ScaleFrameRender(hdc, mX, mY, mFrameX, mFrameY, mSizeX, mSizeY);
	}
}

void Tile::SelectRender(HDC hdc)
{
	Gizmo::GetInstance()->DrawRect(hdc, mRect, Gizmo::Color::Red);
}