#include "pch.h"
#include "LoopEffect.h"

LoopEffect::LoopEffect(wstring keyname, float x, float y)
{
	mImage = IMAGEMANAGER->FindImage(keyname);
	mX = x;
	mY = y;
	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	mAlpha = 1.f;

	Obj->AddObject(ObjectLayer::Effect, this);
}

void LoopEffect::Init()
{
}

void LoopEffect::Update()
{
	mAlpha -= 0.1f;
	if (mAlpha < 0) mIsDestroy = true;
}

void LoopEffect::Render(HDC hdc)
{
	CAMERA->AlphaScaleRender(hdc, mImage, mRect.left, mRect.top, mSizeX, mSizeY, mAlpha);
}

void LoopEffect::Release()
{
}

