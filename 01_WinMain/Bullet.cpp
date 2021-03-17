#include "pch.h"
#include "Bullet.h"

Bullet::Bullet(Image* image,string name, GameObject* object, int damage, float speed, float range, float angle, BulletType type)
	: GameObject(name), mNextTile(nullptr), mImage(image)
{
	mX = object->GetX();
	mY = object->GetY();
	//if (mImage->GetIsFrameImage()) {
	if (mImage == nullptr) {
		mSizeX = 10;
		mSizeY = 10;
	}
	else {
		mSizeX = mImage->GetFrameWidth();
		mSizeY = mImage->GetFrameHeight();
	}
	//}
	//else {
	//	mSizeX = mImage->GetWidth();
	//	mSizeY = mImage->GetHeight();
	//}
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mDamage = damage;
	mAngle = angle;
	mType = type;
	mSpeed = speed;
	mRange = range;
	ObjectManager::GetInstance()->AddObject(ObjectLayer::Player_Bullet, this);
}

void Bullet::Init()
{
	
}

void Bullet::Release()
{

}

void Bullet::Update()
{
	Move();
	if (mRange <= 0) {
		mIsDestroy = true;
	}
}

void Bullet::Render(HDC hdc)
{
	CAMERA->RenderRect(hdc, mRect);
}


void Bullet::Move() {
	mX += mSpeed * cosf(mAngle) * dTime;
	mY -= mSpeed * sinf(mAngle) * dTime;
	mRange -= (fabs(mSpeed * cosf(mAngle) * dTime) + fabs(mSpeed * sinf(mAngle) * dTime));

	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Bullet::Damage(int a) {
	mIsDestroy = true;
}