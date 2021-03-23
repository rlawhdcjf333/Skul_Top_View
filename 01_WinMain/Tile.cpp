#pragma once
#include "pch.h"
#include "Tile.h"
#include "GameObject.h"

Tile::Tile(Image* pImage, float x, float y, int frameX, int frameY, int sizeX, int sizeY, int indexX, int indexY)
	:mX(x), mY(y), mFrameX(frameX), mFrameY(frameY), mSizeX(sizeX), mSizeY(sizeY), mImage(pImage), mIndexX(indexX), mIndexY(indexY), mObject(nullptr), mIsTileEmpty(false)
{
	mTileType = TileType::Normal;

	mRect = RectMake(mX, mY, mSizeX, mSizeY);
	mDiam = DiamMake(mX, mY, mSizeX, mSizeY);
}

void Tile::Update()
{
	if (!mObject) {
		mIsTileEmpty = true;
	}
	else {
		mIsTileEmpty = false;
	}
}


void Tile::Render(HDC hdc)
{
	if (mImage)
	{
		CAMERA->ScaleFrameRender(hdc, mImage, mX, mY, mFrameX, mFrameY, mSizeX, mSizeY);
		//mImage->ScaleFrameRender(hdc, mX, mY, mFrameX, mFrameY, mSizeX, mSizeY);
		
			
	}
	else
	{
		Gizmo::GetInstance()->DrawDiam(hdc, mDiam, Gizmo::Color::Green);
	}
	if (mAttackTest) {
		mTestTime++;
		if (mTestTime > 10) {
			mTestTime = 0;
			mAttackTest = false;
		}
		SelectRenderBlue(hdc);
	}
}
void Tile::AlphaRender(HDC hdc)
{
	if (mImage)
	{
		CAMERA->AlphaScaleFrameRender(hdc, mImage, mX, mY, mFrameX, mFrameY, mSizeX, mSizeY,0.5f);
		//mImage->ScaleFrameRender(hdc, mX, mY, mFrameX, mFrameY, mSizeX, mSizeY);
	}
	else
	{
		Gizmo::GetInstance()->DrawDiam(hdc, mDiam, Gizmo::Color::Green);
	}
	if (mAttackTest) {
		mTestTime++;
		if (mTestTime > 10) {
			mTestTime = 0;
			mAttackTest = false;
		}
		SelectRenderBlue(hdc);
	}
}

void Tile::SelectRender(HDC hdc)
{
	Gizmo::GetInstance()->DrawRect(hdc, mRect, Gizmo::Color::Red);
}

void Tile::SelectRenderBlue(HDC hdc)
{
	Gizmo::GetInstance()->DrawRect(hdc, mRect, Gizmo::Color::Blue);
}

void Tile::SelectRenderMargenta(HDC hdc)
{
	if (mTileType != TileType::Normal) {
		Gizmo::GetInstance()->DrawDiam(hdc, mDiam, Gizmo::Color::Margenta);
	}
}