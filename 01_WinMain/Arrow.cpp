#include "pch.h"
#include "Arrow.h"
#include "Enemy.h"
#include "LoopEffect.h"
#include "Effect.h"

Arrow::Arrow(GameObject* startUnit, int damage, float angle, bool isPiercing)
{
	mName = "Arrow";
	mIsActive = true;
	mIsDestroy = false;

	mIsPiercing = isPiercing;

	IMAGEMANAGER->LoadFromFile(L"Arrow", Resources(L"skul/arrow.bmp"), 30, 30, true);
	mImage = IMAGEMANAGER->FindImage(L"Arrow");


	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();

	mType = BulletType::Straight;
	if (isPiercing) mType = BulletType::Protect;

	mX = startUnit->GetX();
	mY = startUnit->GetY()-15;

	mAngle = angle;
	mDamage = damage;
	mSpeed = isPiercing? 1800:900;
	mRange = isPiercing? 900 :600;
	

	if (mAngle > PI2) mAngle -= PI2;
	if (mAngle < 0) mAngle += PI2;

	if(mIsPiercing) mTargets = Obj->GetObjectList(ObjectLayer::Enemy);

	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	ObjectManager::GetInstance()->AddObject(ObjectLayer::Player_Bullet, this);
}

void Arrow::Update()
{
	if (mRange > 0)
	{
		mX += mSpeed * cosf(mAngle) * dTime;
		mY -= mSpeed * sinf(mAngle) * dTime;
		mRange -= (fabs(mSpeed * cosf(mAngle) * dTime) + fabs(mSpeed * sinf(mAngle) * dTime));

		mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

		if (mIsPiercing)
		{
			for (int i=0; i<mTargets.size(); i++) //피어싱 로직 관통데미지 1회만 적용되게끔
			{
				if (mTargets[i])
				{
					RECT target = mTargets[i]->GetRect();
					if (IntersectRect(&target, &target, &mRect))
					{
						mTargets[i]->Damage(mDamage);
						new Effect(L"SkulHitEffect", target.left, target.top, EffectType::Normal);
						mTargets.erase(mTargets.begin() + i);
						i--;
					}
				}
			}
		}

		new LoopEffect(L"Arrow", mX, mY);
		new LoopEffect(L"Arrow", mX+cosf(mAngle+PI)*7, mY-sinf(mAngle+PI)*7); //반향 잔상 1
		if(mIsPiercing) new LoopEffect(L"Arrow", mX + cosf(mAngle + PI) * 15, mY - sinf(mAngle + PI) * 15); //속도가 빠른 경우 반향 잔상 2
	}

	if (mRange <= 0)
	{
		mIsDestroy = true;
	}

}

void Arrow::Render(HDC hdc)
{
	CAMERA->ScaleRender(hdc, mImage, mRect.left, mRect.top, mSizeX, mSizeY);
}

void Arrow::Release()
{
}
