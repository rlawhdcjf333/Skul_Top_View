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
	int mMark; //표식
	float mAttackReadyDelay; //공격 대기 모션 시간
	bool mAttackEnd; //공격 대미지 여부 체크
	vector<Tile*> mPath; //이동할 타일 스택
	Tile* mTargetTile; //타겟 타일
	int mPathIndex;

public :
	Enemy(int indexX, int indexY);
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
	
	virtual void Damage(int damage) override;
	
	
	void Mark(int damage);
	void setEnemyRect(int x, int y); //Rect 생성 및 mX mY 값 셋팅
};

