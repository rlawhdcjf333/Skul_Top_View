#include "pch.h"
#include "Bullet.h"

Bullet::Bullet(Image* image,string name, GameObject* object, int damage, float speed, float range, float angle, BulletType type)
	: GameObject(name), mImage(image)
{
	mImage = image;
	mX = object->GetX();
	mY = object->GetY();
	if (mImage == nullptr) {
		mSizeX = 10;
		mSizeY = 10;
	}
	else {
		mSizeX = mImage->GetFrameWidth();
		mSizeY = mImage->GetFrameHeight();
	}

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
	if (mRange > 0)
	{
		Move();
	}

	if (mRange <= 0) {
		if (mType == BulletType::SkulHead) return;
		
		mIsDestroy = true;
	}
}

void Bullet::Render(HDC hdc)
{
	if (mImage == nullptr)
	{
		CAMERA->RenderRect(hdc, mRect);
	}
	else
	{
		CAMERA->Render(hdc, mImage, mRect.left, mRect.top);
	}
}


void Bullet::Move() {
	mX += mSpeed * cosf(mAngle) * dTime;
	mY -= mSpeed * sinf(mAngle) * dTime;
	mRange -= (fabs(mSpeed * cosf(mAngle) * dTime) + fabs(mSpeed * sinf(mAngle) * dTime));

	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
}

void Bullet::Damage(int a) {
	if (mType == BulletType::SkulHead) return;
	mIsDestroy = true;
}