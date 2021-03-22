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
	if (mType == BulletType::SkulHead)
	{
		if(TILE[TILELIST->CalcIndexY(mX,mY)][TILELIST->CalcIndexX(mX,mY)]->GetType() == TileType::Block)
		mRange = 0;
	}

	if (mRange > 0)
	{
		Move();
	}

	if (mRange <= 0) {
		if (mType == BulletType::SkulHead) return;
		if (mType == BulletType::Flask) Explosion(mDamage);
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
		if (mImage->GetFrameX() != 0)
		{
			if (RIGHT) CAMERA->ScaleFrameRender(hdc, mImage, mX,mY, 0, 0, 30, 15);
			else if (LEFT) CAMERA->ScaleFrameRender(hdc, mImage,mX, mY, 0, 1, 30, 15);
		}
		else
		{
			CAMERA->Render(hdc, mImage, mRect.left, mRect.top);
		}
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
	if (mType == BulletType::Piercing) return;
	if (mType == BulletType::Flask) Explosion(mDamage);
	mIsDestroy = true;
}

void Bullet::Explosion(int damage)
{
	int indexX = TILELIST->CalcIndexX(mX, mY);
	int indexY = TILELIST->CalcIndexY(mX, mY);

	for (int y = indexY - 2; y <= indexY + 2; y++) {
		for (int x = indexX - 2; x <= indexX + 2; x++) {
			if (y <= 0 || y > TILESizeY || x <= 0 || x > TILESizeX) {
				continue;
			}
			TILE[y][x]->AttackDamage(damage);
		}
	}
}