#include "pch.h"
#include "Door.h"
#include "Animation.h"

Door::Door(int x, int y)
	:GameObject("Door")
{
	mX = x;
	mY = y;
}

void Door::Init()
{
	IMAGEMANAGER->LoadFromFile(L"Door", Resources(L"Door.bmp"), 800, 279, 5,1, true);
	mImage = IMAGEMANAGER->FindImage(L"Door");
	mAnm = new Animation();
	mAnm->InitFrameByStartEnd(0, 0, 4, 0, false);
	mAnm->SetIsLoop(true);
	mAnm->SetFrameUpdateTime(0.3f);
	mAnm->Play();
	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();

	mRect = RectMake(mX, mY, mSizeX, mSizeY);
}

void Door::Release()
{
	SafeDelete(mAnm);
}

void Door::Update()
{
	mAnm->Update();
}

void Door::Render(HDC hdc)
{
	CAMERA->FrameRender(hdc, mImage, mX, mY, mAnm->GetNowFrameX(), mAnm->GetNowFrameY());
	CAMERA->RenderRect(hdc, mRect);
}
