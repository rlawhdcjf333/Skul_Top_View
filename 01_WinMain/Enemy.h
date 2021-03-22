#pragma once
#include "GameObject.h"
class Animation;
enum class Direction:  int {
	left  = 0,
	right = 1
};
struct AnimationPair {
	Image* image;
	Animation* animation;
};

class Enemy : public GameObject
{
//protected:
//	enum StateType{
//		Attack = 0,
//		Idle,
//		Walk,
//		Hit,
//		End
//	};
//	wstring mStateType[(int)StateType::End] = { L"Attack",L"Idle",L"Walk",L"Hit"};
protected:
	class Tile* mTile;
	Player* mCurrentSkul;
	class Animation* mCurrentAnimation;
	Image* mCurrentImage;
	wstring mResources;
	float mAngle;
	int mHp;
	float mAttackDelay;
	Direction mDirection;

public :
	Enemy(int indexX, int indexY);
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
	
	virtual void Damage(int damage) override;

	void setEnemyRect(int x, int y); //Rect 생성 및 mX mY 값 셋팅
};

