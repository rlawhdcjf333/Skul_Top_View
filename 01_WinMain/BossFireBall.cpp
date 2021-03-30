#include "pch.h"
#include "BossFireBall.h"
#include "Effect.h"
#include "Animation.h"

BossFireBall::BossFireBall(float x, float y, float angle, float speed)
	:GameObject()
{
	mX = x;
	mY = y;
	mAngle = angle;
	mSpeed = speed;
	if (LEFT) {
		IMAGEMANAGER->LoadFromFile(L"FileBall(left)", Resources(L"Monster/Boss/FireBall(left).bmp"), 1200, 114, 12, 2, true);
		mImage = IMAGEMANAGER->FindImage(L"FileBall(left)");
	}
	else {
		IMAGEMANAGER->LoadFromFile(L"FileBall(right)", Resources(L"Monster/Boss/FireBall(right).bmp"), 1200, 114, 12, 2, true);
		mImage = IMAGEMANAGER->FindImage(L"FileBall(right)");
	}
	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mAnimation = new Animation();
	mAnimation->InitFrameByStartEnd(0,0,mImage->GetFrameX()-1,mImage->GetFrameY()-1,false);
	mAnimation->SetFrameUpdateTime(0.1f);
	mAnimation->SetIsLoop(true);
	mAnimation->Play();
	Obj->AddObject(ObjectLayer::Enemy_Bullet, this);
}

void BossFireBall::Init()
{
}

void BossFireBall::Update()
{
	mAnimation->Update();
	mX += cos(mAngle) * mSpeed * dTime;
	mY -= sin(mAngle) * mSpeed * dTime;
	mMove += abs(cos(mAngle) * mSpeed * dTime) + abs(sin(mAngle) * mSpeed * dTime);
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	RECT rc = SKUL->GetCurrentSkul()->GetHitBox();
	RECT temp;
	if (IntersectRect(&temp, &mRect, &rc)) {
		SKUL->Damage(5);
		new Effect(L"Boss_BossAttackEffect", mX, mY, EffectType::Normal);
		mIsDestroy = true;
	}
	else if (mMove > 1200.f) {
		new Effect(L"Boss_BossAttackEffect", mX, mY, EffectType::Normal);
		mIsDestroy = true;
	}
}

void BossFireBall::Render(HDC hdc)
{
	CAMERA->FrameRender(hdc, mImage,mRect.left,mRect.top,mAnimation->GetNowFrameX(),mAnimation->GetNowFrameY());
}

void BossFireBall::Release()
{
	SafeDelete(mAnimation)
}
