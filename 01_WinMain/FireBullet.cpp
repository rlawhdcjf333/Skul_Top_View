#include "pch.h"
#include "FireBullet.h"
#include "LoopEffect.h"
#include "Enemy.h"
#include "Effect.h"

FireBullet::FireBullet(Player* startUnit, float damage)
{
	mName = "FireBulllet";
	mIsActive = true;
	mIsDestroy = false;

	IMAGEMANAGER->LoadFromFile(L"FireBullet", Resources(L"item/fireBullet.bmp"), 75, 78, true);
	mImage = IMAGEMANAGER->FindImage(L"FireBullet");
	IMAGEMANAGER->LoadFromFile(L"FireSlash", Resources(L"item/fireSlash.bmp"), 500, 400, 5, 4, true);

	mSizeX = 45;
	mSizeY = 45;

	mType = BulletType::Straight;

	mAngle = PI / 2;
	mDamage = damage;
	mSpeed = 300;

	mX = startUnit->GetX();
	mY = startUnit->GetY();

	mTarget == nullptr;


	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	ObjectManager::GetInstance()->AddObject(ObjectLayer::Player_Bullet, this);

}

void FireBullet::Update()
{

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
		mIsDestroy = true;
	}


	mX += mSpeed * dTime * cosf(mAngle);
	mY -= mSpeed * dTime * sinf(mAngle);

	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

}

void FireBullet::Release()
{
	if (mTarget)
	{
		(new Effect(L"FireSlash", mTarget->GetX(), mTarget->GetY(), EffectType::Follow))->Scaling(120,120);
	}
	else
	{
		(new Effect(L"FireSlash", mX, mY, EffectType::Normal))->Scaling(120, 120);
	}

}

void FireBullet::Render(HDC hdc)
{
	CAMERA->ScaleRender(hdc, mImage, mRect.left, mRect.top, mSizeX, mSizeY);
}
