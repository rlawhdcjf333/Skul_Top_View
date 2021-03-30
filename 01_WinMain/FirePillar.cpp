#include "pch.h"
#include "Animation.h"
#include "FirePillar.h"

FirePillar::FirePillar(int x, int y, RECT rc)
	:GameObject(), mDamageCheck(true)
{
	mX = x + 30;
	mY = y + 15;
	mRect = rc;
	IMAGEMANAGER->LoadFromFile(L"FirePillar", Resources(L"Monster/FirePillar.bmp"), 1452, 420,12,3, true);
	mImage = IMAGEMANAGER->FindImage(L"FirePillar");
	mAnimtion = new Animation();
	mAnimtion->InitFrameByStartEnd(0,0,11,2,false);
	mAnimtion->SetFrameUpdateTime(0.1f);
	mAnimtion->Play();
	Obj->AddObject(ObjectLayer::Enemy_Bullet,this);
}

void FirePillar::Init()
{
}

void FirePillar::Update()
{
	mAnimtion->Update(); //4 13
	if (mAnimtion->GetIsPlay()) {
		if (mAnimtion->GetCurrentFrameIndex()>3 && mAnimtion->GetCurrentFrameIndex()<13) {
			if (mDamageCheck) {
				RECT rc = SKUL->GetCurrentSkul()->GetHitBox();
				RECT temp;
				if (IntersectRect(&mRect,&rc,&temp)) {
					mDamageCheck = false;
					SKUL->Damage(10);
				}
			}
		}
	}
	else {
		mIsDestroy = true;
	}
}

void FirePillar::Render(HDC hdc)
{
	CAMERA->CenterBottomFrameRender(hdc, mImage,mX,mRect.bottom,mAnimtion->GetNowFrameX(),mAnimtion->GetNowFrameY());
}

void FirePillar::Release()
{
	SafeDelete(mAnimtion)
}
