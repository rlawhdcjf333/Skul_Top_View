#include "pch.h"
#include "ClownDagger.h"
#include "Effect.h"

ClownDagger::ClownDagger(GameObject* obj, int damage, float angle, float range, bool isExplosive)
{
	mName = "ClownDagger";
	mIsActive = true;
	mIsDestroy = false;

	mIsExplosive = false;
	if (isExplosive) mIsExplosive = true;

	IMAGEMANAGER->LoadFromFile(L"ClownDagger", Resources(L"skul/dagger.bmp"), 11, 8, 1,2, true);
	mImage = IMAGEMANAGER->FindImage(L"ClownDagger");

	IMAGEMANAGER->LoadFromFile(L"DaggerExplosion", Resources(L"skul/daggerExplosion.bmp"), 4600, 200, 23, 1, true);

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();

	mType = BulletType::Mark;
	if (isExplosive) mType = BulletType::Flask;

	mX = obj->GetX();
	mY = obj->GetY();

	if (angle > PI / 2 and angle <= 3 * PI / 2) //ÁÂÃø
	{
		mFrameY = 1;
	}
	else if (angle > 3 * PI / 2 or angle <= PI / 2) //¿ìÃø
	{
		mFrameY = 0;
	}
	

	mSpeed = 500;
	mDamage = damage;
	mRange = range;
	mAngle = angle;

	if (mAngle > PI2) mAngle -= PI2;
	if (mAngle < 0) mAngle += PI2;

	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	ObjectManager::GetInstance()->AddObject(ObjectLayer::Player_Bullet, this);

}

void ClownDagger::Update()
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
		if (mType == BulletType::Flask)	Explosion(2*mMagicalAttackPower, 2, [this]() {new Effect(L"DaggerExplosion", mX, mY, EffectType::Normal);});
		mIsDestroy = true;
	}
}

void ClownDagger::Render(HDC hdc)
{
	if(RIGHT) CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, 0,0,22,8);
	if(LEFT) CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, 0,1,22,8);
}

void ClownDagger::Damage(int val)
{
	if (mType == BulletType::Flask)
	{
		Explosion(mDamage, 2, [this]() {new Effect(L"DaggerExplosion", mX, mY, EffectType::Normal);});
	}
	mIsDestroy = true;
}