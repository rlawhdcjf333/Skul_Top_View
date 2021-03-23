#include "pch.h"
#include "Enemy.h"
#include "Animation.h"


Enemy::Enemy(int indexX, int indexY)
	:GameObject(), mCurrentAnimation(nullptr), mCurrentImage(nullptr), mAngle(0), mAttackReadyDelay(0), mAttackEnd(true), mPathIndex(0)
{
	mIndexX = indexX;
	mIndexY = indexY;
	mX = TILE[mIndexY][mIndexX]->GetX() + TileSizeX / 2;
	mY = TILE[mIndexY][mIndexX]->GetY() + TileSizeY / 2;
}

void Enemy::Init()
{
}

void Enemy::Update()
{
}

void Enemy::Render(HDC hdc)
{
}

void Enemy::Release()
{
}

void Enemy::Damage(int damage)
{
	
}
void Enemy::setEnemyRect(int x, int y)
{
	mX = x;
	mY = y;
	mRect = RectMakeBottom(mX,mY,mSizeX, mSizeY);
}


void Enemy::Mark(int damage) {
	mMark++;
	mHp -= damage;
	if (mMark >= 3 && mHp > 0) {
		mHp -= damage;
	}
}