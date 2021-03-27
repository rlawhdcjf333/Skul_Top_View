#include "pch.h"
#include "Hwadun.h"
#include "Animation.h"
#include "Enemy.h"
#include "Burning.h"

Hwadun::Hwadun(GameObject* startUnit, float damage, float angle)
{
	mName = "Hwadun";
	mIsActive = true;
	mIsDestroy = false;

	IMAGEMANAGER->LoadFromFile(L"Hwadun", Resources(L"skul/hwadun.bmp"), 1100, 200, 11, 2, true);
	mImage = IMAGEMANAGER->FindImage(L"Hwadun");

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();

	mType = BulletType::Protect;

	mX = startUnit->GetX();
	mY = startUnit->GetY();
	
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

	mDamage = damage;

	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mAnimation = new Animation(0, 0, 10, 1, false, false, 0.05f, [this]() { mAnimation->SetCurrentFrameIndex(21); mIsDestroy = true;});
	mAnimation->Play();

	ObjectManager::GetInstance()->AddObject(ObjectLayer::Player_Bullet, this);
}

void Hwadun::Update()
{
	mAnimation->Update();
	if (mAnimation->GetCurrentFrameIndex()==2)
	{
		Explosion(mDamage, 2);
		for (auto elem : Obj->GetObjectList(ObjectLayer::Enemy))
		{
			Enemy* downcast = (Enemy*)elem;
			if (downcast->GetHitTime() == 0.6f)
			{
				new Burning(elem, mDamage, 5);
			}
		}
	}
}

void Hwadun::Release()
{
	SafeDelete(mAnimation)
}

void Hwadun::Render(HDC hdc)
{
	CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mAnimation->GetNowFrameX(), mAnimation->GetNowFrameY(), 200, 200);
}
