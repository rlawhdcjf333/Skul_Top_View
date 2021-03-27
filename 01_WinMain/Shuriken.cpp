#include "pch.h"
#include "Shuriken.h"
#include "Effect.h"

Shuriken::Shuriken(GameObject* obj, int damage, float angle, float range)
{
	mName = "Shuriken";
	mIsActive = true;
	mIsDestroy = false;

	IMAGEMANAGER->LoadFromFile(L"Shuriken", Resources(L"skul/shuriken.bmp"), 19,19, true);
	mImage = IMAGEMANAGER->FindImage(L"Shuriken");


	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();

	mType = BulletType::Straight;

	mX = obj->GetX();
	mY = obj->GetY()-15;

	if (angle > 7 * PI / 4 or angle <= PI / 4) //우측
	{
		mX += mSizeX;
	}
	else if (angle > 3 * PI / 4 and angle <= 5 * PI / 4) //좌측
	{
		mX -= mSizeX;
	}
	else if (angle > PI / 4 and angle <= 3 * PI / 4) //상방
	{
		mY -= mSizeY;
	}
	else if (angle > 5 * PI / 4 and angle <= 7 * PI / 4)//하방
	{
		mY += mSizeY;
	}

	mSpeed = 500;
	mDamage = damage;
	mRange = range;
	mAngle = angle - PI / 3 + (Random::GetInstance()->RandomInt(8)) * PI / 12;

	if (mAngle > PI2) mAngle -= PI2;
	if (mAngle < 0) mAngle += PI2;

	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	ObjectManager::GetInstance()->AddObject(ObjectLayer::Player_Bullet, this);

}

void Shuriken::Update()
{
	if (mRange > 0)
	{
		mX += mSpeed * cosf(mAngle) * dTime;
		mY -= mSpeed * sinf(mAngle) * dTime;
		mRange -= (fabs(mSpeed * cosf(mAngle) * dTime) + fabs(mSpeed * sinf(mAngle) * dTime));

		mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	}

	if (mRange <= 0) 
	{
		mIsDestroy = true;
	}
}

void Shuriken::Render(HDC hdc)
{
	CAMERA->ScaleRender(hdc, mImage, mRect.left, mRect.top, 20, 20);
}

void Shuriken::Release()
{
	(new Effect(L"NinjaHit", mX, mY, EffectType::Normal))->Scaling(75, 75);
}