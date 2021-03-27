#include "pch.h"
#include "GrimSoul.h"
#include "Enemy.h"
#include "Animation.h"
#include "Effect.h"

GrimSoul::GrimSoul(Enemy* startUnit, float damage)
{
	mName = "GrimSoul";
	mIsActive = true;
	mIsDestroy = false;

	IMAGEMANAGER->LoadFromFile(L"GrimSoul", Resources(L"skul/grimSoul.bmp"), 500,50,10,1, true);
	mImage = IMAGEMANAGER->FindImage(L"GrimSoul");

	IMAGEMANAGER->LoadFromFile(L"SoulBurn", Resources(L"skul/soulBurn.bmp"), 500, 500, 5, 5, true);

	mSizeX = 45;
	mSizeY = 45;

	mType = BulletType::Straight;

	mAngle = PI / 2;
	mDamage = damage;
	mSpeed = 100;

	mX = RAND->RandomInt(2) == 0 ? startUnit->GetX() + RAND->RandomInt(30, 60) : startUnit->GetX() - RAND->RandomInt(30, 60);
	mY = RAND->RandomInt(2) == 0 ? startUnit->GetY() + RAND->RandomInt(30, 60) : startUnit->GetY() - RAND->RandomInt(30, 60);

	mTarget == nullptr;

	mAnimation = new Animation(0, 0, 9, 0, false, true, 0.1f);
	mAnimation->Play();

	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mDuration = 3;

	ObjectManager::GetInstance()->AddObject(ObjectLayer::Player_Bullet, this);
}

void GrimSoul::Update()
{
	mDuration -= dTime;
	if (mDuration < 0) mIsDestroy = true;

	auto EnemyList = Obj->GetObjectList(ObjectLayer::Enemy);
	if (EnemyList.size() > 0)
	{
		mMinDistance = Math::GetDistance(mX, mY, EnemyList[0]->GetX(), EnemyList[0]->GetY());
		mTarget = dynamic_cast<Enemy*>(EnemyList[0]);

		for (GameObject* elem : EnemyList)
		{
			float dist = Math::GetDistance(mX, mY, elem->GetX(), elem->GetY());
			if (mMinDistance > dist)
			{
				mMinDistance = dist;
				mTarget = dynamic_cast<Enemy*>(elem);
			}
		}
		mAngle = Math::GetAngle(mX, mY, mTarget->GetX(), mTarget->GetY());
	}
	else
	{
		mTarget = nullptr;
		mAngle = PI / 2;
	}

	mX += mSpeed * dTime * cosf(mAngle);
	mY -= mSpeed * dTime * sinf(mAngle);

	mAnimation->Update();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void GrimSoul::Release()
{
	Explosion(mDamage,1);
	(new Effect(L"SoulBurn", mX, mY, EffectType::Normal))->Scaling(45, 45);
	SafeDelete(mAnimation)
}

void GrimSoul::Render(HDC hdc)
{
	CAMERA->AlphaScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mAnimation->GetNowFrameX(), 0, mSizeX, mSizeY, 0.7f);
}
