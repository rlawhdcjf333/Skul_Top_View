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
	enum class StateType : int {
		Attack= 0,
		Idle,
		Walk,
		Hit,
		End
	};
	wstring mStateType[(int)StateType::End] = { L"Attack",L"Idle",L"Walk",L"Hit"};
protected:
	class Tile* mTile;
	class Animation* mCurrentAnimation;
	Image* mCurrentImage;
	map<StateType, AnimationPair> mAnimationMap[2];
	wstring mResources;
public :
	Enemy(int indexX, int indexY);
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release() override;
	
	void AnimationSet(wstring filename);
	void Damge(int damage);
};

