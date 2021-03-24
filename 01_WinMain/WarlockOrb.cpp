#include "pch.h"
#include "WarlockOrb.h"
#include "Animation.h"

WarlockOrb::WarlockOrb(GameObject* startUnit, float speed, float angle, float range, float damage, bool isCompleted)
{
	mName = "WarlockOrb";
	mIsActive = true;
	mIsDestroy = false;

	mType = BulletType::Protect;
	mX = startUnit->GetX();
	mY = startUnit->GetY() - 15;

	mSpeed = speed;
	mAngle = angle;
	mRange = range;
	mDamage = damage;
	mIsCompleted = isCompleted;
	

	if (isCompleted)
	{
		IMAGEMANAGER->LoadFromFile(L"OrbComp", Resources(L"skul/orbComp.bmp"), 1000, 200, 10, 2, true);
		mImage = IMAGEMANAGER->FindImage(L"OrbComp");
	}
	else
	{
		IMAGEMANAGER->LoadFromFile(L"OrbIncomp", Resources(L"skul/orbIncomp.bmp"), 1000, 200, 10, 2, true);
		mImage=IMAGEMANAGER->FindImage(L"OrbIncomp");
	}

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();

	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	mAnimation = new Animation(0, 0, 9, 1, false, false, 0.05f, [this]() {mAnimation->Play();});
	mAnimation->Play();

	ObjectManager::GetInstance()->AddObject(ObjectLayer::Player_Bullet, this);

}

void WarlockOrb::Update()
{
	if (mAnimation->GetCurrentFrameIndex() == 1 or mAnimation->GetCurrentFrameIndex() ==10)
	{
		if(mAnimation->GetCurrentFrameTime() < dTime)
		Explosion(mDamage, 2);
	}

	if (mRange > 0)
	{
		Move();
		mAnimation->Update();
		mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	}
	else
	{
		mIsDestroy = true;
	}

}

void WarlockOrb::Release()
{
	SafeDelete(mAnimation);
}

void WarlockOrb::Render(HDC hdc)
{
	CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top-50, mAnimation->GetNowFrameX(), mAnimation->GetNowFrameY(), 200, 200);

}
