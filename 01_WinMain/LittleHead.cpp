#include "pch.h"
#include "LittleHead.h"

LittleHead::LittleHead(GameObject* obj, int damage, float angle, float range)
{
	mName = "LittleHead";
	mIsActive = true;
	mIsDestroy = false;

	IMAGEMANAGER->LoadFromFile(L"LittleHead", Resources(L"/skul/head.bmp"), 15, 13, true);
	mImage = IMAGEMANAGER->FindImage(L"LittleHead");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();

	mType = BulletType::Straight;

	mX = obj->GetX();
	mY = obj->GetY();

	mInitThrow = 200;
	mSpeed = 500;
	mDamage = damage;
	mRange = range;
	mAngle = angle;

	if (mAngle > PI2) mAngle -= PI2;
	if (mAngle < 0) mAngle += PI2;

	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	ObjectManager::GetInstance()->AddObject(ObjectLayer::Player_Bullet, this);

}

void LittleHead::Update()
{
	if (TILE[TILELIST->CalcIndexY(mX, mY)][TILELIST->CalcIndexX(mX, mY)]->GetType() == TileType::Block)
	{
		Damage(0);
	}

	if (mRange > 0)
	{
		mX += mSpeed * cosf(mAngle) * dTime;
		mY -= mSpeed * sinf(mAngle) * dTime;
		mRange -= (fabs(mSpeed * cosf(mAngle) * dTime) + fabs(mSpeed * sinf(mAngle) * dTime));

		mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	}

	if (mRange <= 0)
	{

		if (mY <= mDestinationY)
		{
			mInitThrow -= mSpeed * dTime;
			mY -= mInitThrow * dTime;
			mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
		}

		mType = BulletType::Protect;
	}
}

void LittleHead::Render(HDC hdc)
{
	CAMERA->ScaleRender(hdc, mImage, mRect.left, mRect.top, mSizeX, mSizeY);
}

void LittleHead::Damage(int val)
{
	mDestinationY = mY;
	mRange = 0;
}