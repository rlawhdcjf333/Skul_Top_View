#include "pch.h"
#include "Bullet.h"
#include "Effect.h"

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

	mDamage = damage;
	mAngle = angle;
	mType = type;
	mSpeed = speed;
	mRange = range;
	mCurrentFrameX = 0;
	mCurrentFrameY = 0;
	mFrameTick = 0.1f;
	
	

	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);



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
	if (mType == BulletType::FrameProjectile)
	{
		mFrameTick-= dTime;
		if (mFrameTick < 0)
		{
			mFrameTick = 0.1f;
			mCurrentFrameX++;
			if (mCurrentFrameX >= mImage->GetFrameX())
			{
				mCurrentFrameX = 0;
			}
		}

	}

	if (mType == BulletType::Barricade)
	{
		mFrameTick -= dTime;
		if (mFrameTick < 0)
		{
			mFrameTick = 0.05f;
			mCurrentFrameX++;
			if (mCurrentFrameX >= mImage->GetFrameX())
			{
				Explosion(SKUL->GetCurrentSkul()->GetMagicalAttackPower(), 3);
				mCurrentFrameY++;
				mCurrentFrameX = 0;
			}
			if (mCurrentFrameY >= mImage->GetFrameY())
			{
				mIsDestroy = true;
			}
		}

	}

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
		if (mType == BulletType::Flask)
		{
			if (mName == "FireFlask") Explosion(mDamage, 2, [this]() {new Effect(L"Fire", mX, mY, EffectType::Normal);});
			else if (mName == "DiseaseFlask") Explosion(mDamage, 2, [this]() {new Effect(L"Disease",mX,mY, EffectType::Normal);});
			else Explosion(mDamage);
		}
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
		if (mType == BulletType::FrameProjectile)
		{
			if (RIGHT) CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top-20, mCurrentFrameX, 0, 30, 15);
			else if (LEFT) CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top-20, mCurrentFrameX, 1, 30, 15);
			return;
		}

		if (mType == BulletType::Barricade)
		{
			int indexX = TILELIST->CalcIndexX(mX, mY);
			int indexY = TILELIST->CalcIndexY(mX, mY);
			CAMERA->AlphaScaleFrameRender(hdc, mImage, TILE[indexY][indexX]->GetX() + TileSizeX / 2 -200, TILE[indexY][indexX]->GetY() + TileSizeY / 2 -150, 
				mCurrentFrameX, mCurrentFrameY, 400,200, 0.5f);
			return;
		}

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
	if (mType == BulletType::Barricade) return;
	if (mType == BulletType::Flask)
	{
		if (mName == "FireFlask") Explosion(mDamage, 2, [this]() {new Effect(L"Fire", mX, mY, EffectType::Normal);});
		else if (mName == "DiseaseFlask") Explosion(mDamage, 2, [this]() {new Effect(L"Disease", mX, mY, EffectType::Normal);});
		else Explosion(mDamage);
	}
	mIsDestroy = true;
}

void Bullet::Explosion(int damage, int range, function <void(void)> func) //기본 폭발반경 2타일
{
	int indexX = TILELIST->CalcIndexX(mX, mY);
	int indexY = TILELIST->CalcIndexY(mX, mY);

	for (int y = indexY - range; y <= indexY + range; y++) {
		for (int x = indexX - range; x <= indexX + range; x++) {
			if (y <= 0 || y > TILESizeY || x <= 0 || x > TILESizeX) {
				continue;
			}
			TILE[y][x]->AttackDamage(damage);
		}
	}
	func();
}