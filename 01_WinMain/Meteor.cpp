#include "pch.h"
#include "Meteor.h"
#include "Animation.h"
#include "Effect.h"

Meteor::Meteor(GameObject* startUnit, float speed, float angle, float damage, bool isCompleted)
{
	mName = "Meteor";
	mIsActive = true;
	mIsDestroy = false;

	mType = BulletType::Protect;
	mX = startUnit->GetX() + 600;
	mY = startUnit->GetY() - 600;

	mSpeed = speed;
	mAngle = angle;
	mRange = 1200;
	mAngle = Math::GetAngle(mX, mY, startUnit->GetX(), startUnit->GetY());
	mDamage = damage;
	mIsCompleted = isCompleted;

	IMAGEMANAGER->LoadFromFile(L"MeteorBang", Resources(L"skul/meteorBang.bmp"), 7200, 200, 36, 1, true);
	if (isCompleted)
	{
		IMAGEMANAGER->LoadFromFile(L"MeteorComp", Resources(L"skul/meteorComp.bmp"), 2000, 1000, 4, 2, true);
		mImage = IMAGEMANAGER->FindImage(L"MeteorComp");
	}
	else
	{
		IMAGEMANAGER->LoadFromFile(L"MeteorIncomp", Resources(L"skul/meteorIncomp.bmp"), 1800, 400, 9, 2, true);
		mImage = IMAGEMANAGER->FindImage(L"MeteorIncomp");
	}

	mSizeX = 200;
	mSizeY = 200;

	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	mAnimation = new Animation(0, 0, mImage->GetFrameX()-1, mImage->GetFrameY()-1, false, false, 0.05f, [this]() {mAnimation->Play();});
	mAnimation->Play();

	ObjectManager::GetInstance()->AddObject(ObjectLayer::Player_Bullet, this);

}

void Meteor::Update()
{
	if (mRange > 0)
	{
		Move();
		mAnimation->Update();
		mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	}
	else
	{
		if (mIsCompleted)
		{
			Explosion(mDamage, 5, [this]() {(new Effect(L"MeteorBang", mX-100, mY-100, EffectType::Normal))->Scaling(400,400);});
			CAMERA->PanningOn(10);
		}
		else
		{
			Explosion(mDamage, 3, [this]() {new Effect(L"MeteorBang", mX, mY, EffectType::Normal);});
			CAMERA->PanningOn(5);
		}
		mIsDestroy = true;
	}

}

void Meteor::Release()
{
	SafeDelete(mAnimation);
}

void Meteor::Render(HDC hdc)
{
	CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mAnimation->GetNowFrameX(), mAnimation->GetNowFrameY(), mSizeX, mSizeY);

}
