#include "pch.h"
#include "Enemy.h"
#include "Animation.h"
#include "GrimSoul.h"


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


	if (SKUL->GetCurrentSkul()->GetKeyName()==L"GrimReaper")
	{
		new GrimSoul(this, SKUL->GetCurrentSkul()->GetMagicalAttackPower());
		new GrimSoul(this, SKUL->GetCurrentSkul()->GetMagicalAttackPower());	
		new GrimSoul(this, SKUL->GetCurrentSkul()->GetMagicalAttackPower());
	}
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


void Enemy::Mark(int damage, function <void(void)> func) {
	mMark++;
	mHp -= damage;
	if (mMark >= 3 && mHp > 0) {
		mMark = 0;
		func();
	}
}

void Enemy::Explosion(int damage, int range) //기본 폭발반경 2타일
{
	for (int y = mIndexY - range; y <= mIndexY + range; y++) {
		for (int x = mIndexX - range; x <= mIndexX + range; x++) {
			if (y <= 0 || y > TILESizeY || x <= 0 || x > TILESizeX) {
				continue;
			}
			TILE[y][x]->AttackDamage(damage);
		}
	}
}