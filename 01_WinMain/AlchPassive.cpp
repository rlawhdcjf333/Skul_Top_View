#include "pch.h"
#include "AlchPassive.h"
#include "Animation.h"
#include "Effect.h"

AlchPassive::AlchPassive(GameObject* startUnit, float damage, float angle)
{
	mName = "GolemBullet";
	mIsActive = true;
	mIsDestroy = false;

	IMAGEMANAGER->LoadFromFile(L"GolemBullet", Resources(L"skul/golemBullet.bmp"), 600, 600, 6, 6, true);
	mImage = IMAGEMANAGER->FindImage(L"GolemBullet");

	IMAGEMANAGER->LoadFromFile(L"GolemBulletDespawn", Resources(L"skul/golemBulletDespawn.bmp"), 1400,200,7,1, true);

	mSizeX = 200;
	mSizeY = 200;
	mSpeed = 150;
	mType = BulletType::Protect;
	mAngle = angle;

	mX = startUnit->GetX();
	mY = startUnit->GetY();

	mDamage = damage;

	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	mAnimation = new Animation(0, 0, 5, 5, false, false, 0.05f, [this]() {mIsDestroy = true; new Effect(L"GolemBulletDespawn", mX, mY, EffectType::Normal);});
	mAnimation->Play();

	ObjectManager::GetInstance()->AddObject(ObjectLayer::Player_Bullet, this);
}

void AlchPassive::Update()
{
	mX += mSpeed * cosf(mAngle) * dTime;
	mY -= mSpeed * sinf(mAngle) * dTime;
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	mAnimation->Update();
	if (mAnimation->GetCurrentFrameIndex() % 6 == 1 and mAnimation->GetCurrentFrameTime()> 0.05f-dTime)
	{
		Explosion(mDamage, 2);
	}
}

void AlchPassive::Release()
{
	SafeDelete(mAnimation)
}

void AlchPassive::Render(HDC hdc)
{
	CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mAnimation->GetNowFrameX(), mAnimation->GetNowFrameY(), mSizeX, mSizeY);
}
