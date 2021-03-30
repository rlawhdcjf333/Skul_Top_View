#include "pch.h"
#include "EnemyArrow.h"
#include "Effect.h"

EnemyArrow::EnemyArrow(float x, float y, float angle , float speed)
	:GameObject(),mMove(0)
{
	IMAGEMANAGER->LoadFromFile(L"EnemyArrow", Resources(L"Monster/Arrow.bmp"), 22, 14, 1, 2, true);
	mImage = IMAGEMANAGER->FindImage(L"EnemyArrow");
	mX = x;
	mY = y;
	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mAngle = angle;
	mSpeed = speed;
	Obj->AddObject(ObjectLayer::Enemy_Bullet,this);
}

void EnemyArrow::Init()
{
}

void EnemyArrow::Update()
{
	mX += cos(mAngle) * mSpeed * dTime;
	mY -= sin(mAngle) * mSpeed * dTime;
	mMove += abs(cos(mAngle) * mSpeed * dTime) + abs(sin(mAngle) * mSpeed * dTime);
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	RECT rc = SKUL->GetCurrentSkul()->GetHitBox();
	RECT temp;
	if (IntersectRect(&temp,&mRect,&rc)) {
		SKUL->Damage(4);
		mIsDestroy = true;
		new Effect(L"Boss_BossAttackEffect", mX, mY, EffectType::Normal);
	}
	else if (mMove > 1200.f) {
		mIsDestroy = true;
		new Effect(L"Boss_BossAttackEffect", mX, mY, EffectType::Normal);
	}
}

void EnemyArrow::Render(HDC hdc)
{
	CAMERA->RenderRect(hdc, mRect);
	if (LEFT) {
		CAMERA->FrameRender(hdc, mImage, mRect.left, mRect.top, 0, 0);
	}
	else {
		CAMERA->FrameRender(hdc, mImage, mRect.left, mRect.top, 0, 1);
	}
	
}

void EnemyArrow::Release()
{
}
