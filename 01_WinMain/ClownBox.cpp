#include "pch.h"
#include "ClownBox.h"
#include "Effect.h"

ClownBox::ClownBox(GameObject* obj, int damage, float angle)
{
	mName = "ClownBox";
	mIsActive = true;
	mIsDestroy = false;

	IMAGEMANAGER->LoadFromFile(L"ClownBox", Resources(L"/skul/box.bmp"), 26, 27, true);
	mImage = IMAGEMANAGER->FindImage(L"ClownBox");

	IMAGEMANAGER->LoadFromFile(L"ShowTime", Resources(L"/skul/boxBlowUp.bmp"), 2600, 200, 13,1,true);

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();

	mType = BulletType::Protect;

	mX = obj->GetX();
	mDestinationY = obj->GetY();
	mY = mDestinationY + 15;

	mInitThrow = 50;
	mSpeed = 100;
	mDamage = damage;

	if (mAngle > PI2) mAngle -= PI2;
	if (mAngle < 0) mAngle += PI2;

	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	mShowTime = 1.f;
	ObjectManager::GetInstance()->AddObject(ObjectLayer::Player_Bullet, this);
}

void ClownBox::Update()
{
	mShowTime -= dTime;

	if (mY > mDestinationY)
	{
		mInitThrow -= mSpeed*dTime;
		mY -= mInitThrow*dTime;
		mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	}

	if (mShowTime < 0)
	{
		Explosion(mDamage, 2, [this]() {new Effect(L"ShowTime", mX, mY, EffectType::Normal);});
		mIsDestroy = true;
	}
}

void ClownBox::Render(HDC hdc)
{
	CAMERA->Render(hdc, mImage, mRect.left, mRect.top);
}
