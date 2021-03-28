#include "pch.h"
#include "Door.h"
#include "Animation.h"

Door::Door(int x, int y)
{
	mX = x;
	mY = y;
}

void Door::Init()
{
	IMAGEMANAGER->LoadFromFile(L"Door", Resources(L"Door.bmp"), 800, 279, 5,1, true);
	mImage = new Image;
	mImage = IMAGEMANAGER->FindImage(L"Door");
	mCurrentAnm = new Animation();
	mCurrentAnm->InitFrameByStartEnd(0, 0, 4, 0, false);
	mCurrentAnm->SetFrameUpdateTime(0.1f);
}

void Door::Release()
{
	SafeDelete(mImage);
}

void Door::Update()
{
	//mCurrentAnm->Update();
}

void Door::Render(HDC hdc)
{
	CAMERA->FrameRender(hdc, mImage, mX, mY, mCurrentAnm->GetNowFrameX(), mCurrentAnm->GetNowFrameY());
}
