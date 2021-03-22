#include "pch.h"
#include "Enemy.h"
#include "Animation.h"


Enemy::Enemy(int indexX, int indexY)
	:GameObject(), mCurrentAnimation(nullptr), mCurrentImage(nullptr), mAngle(0)
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
//void Enemy::AnimationSet() {
//	//¿ÞÂÊ
//	for (int a = 0; a < (int)StateType::End; a++) {
//		StateType type = (StateType)a;
//		Image* image = IMAGEMANAGER->FindImage(Resources(mResources+mStateType[a]));
//		Animation* animation = new Animation();
//		animation->InitFrameByStartEnd(0,(int)Direction::left,image->GetFrameX(),image->GetFrameY(),false);
//		AnimationPair aPair = {image,animation};
//		mAnimationMap[(int)Direction::left].insert(make_pair(type, aPair));
//	}
//	//¿À¸¥ÂÊ
//	for (int a = 0; a < (int)StateType::End; a++) {
//		StateType type = (StateType)a;
//		Image* image = IMAGEMANAGER->FindImage(Resources(mResources + mStateType[a]));
//		Animation* animation = new Animation();
//		animation->InitFrameByStartEnd(0, (int)Direction::right, image->GetFrameX(), image->GetFrameY(), false);
//		AnimationPair aPair = { image,animation };
//		mAnimationMap[(int)Direction::right].insert(make_pair(type, aPair));
//	}
//}
