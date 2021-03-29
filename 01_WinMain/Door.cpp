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
	IMAGEMANAGER->LoadFromFile(L"Door", Resources(L"Door.bmp"), 1600, 279, 10,1, true);
	mImage = IMAGEMANAGER->FindImage(L"Door");
	mAnm1 = new Animation();
	mAnm1->InitFrameByStartEnd(0, 0, 4, 0, false);
	mAnm1->SetIsLoop(true);
	mAnm1->SetFrameUpdateTime(0.1f);
	mCurrentAnm = mAnm1;
	mCurrentAnm->Play();

	mAnm2 = new Animation();
	mAnm2->InitFrameByStartEnd(5, 0, 9, 0, false);
	mAnm2->SetIsLoop(true);
	mAnm2->SetFrameUpdateTime(0.3f);

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();

	mRect = RectMake(mX, mY, mSizeX, mSizeY);
}

void Door::Release()
{
	SafeDelete(mAnm1);
	SafeDelete(mAnm2);
}

void Door::Update()
{
	mCurrentAnm->Update();
	if (mCurrentAnm->GetNowFrameX() == 4)
	{
		mCurrentAnm->Stop();
		mCurrentAnm = mAnm2;
		mCurrentAnm->Play();
	}
}

void Door::Render(HDC hdc)
{
	CAMERA->FrameRender(hdc, mImage, mX, mY, mCurrentAnm->GetNowFrameX(), 0);
}
