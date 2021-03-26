#pragma once
#include "Player.h"

class Animation;
class Alchemist : public Player
{
	Image* mProjectile;
	Image* mFlask;

	GameObject* mAttackTarget;
	float mMinDistance;
	float mAttackCoolTime;

	float mGolemCoolTime;

	enum class Motion : int {
		rightIdle = 0,
		leftIdle,
		rightWalk,
		leftWalk,
		rightDash,
		leftDash,

		rightSkill1,
		leftSkill1,
		rightSkill2,
		leftSkill2,

		End
	};

	Animation* mAnimationList[(int)Motion::End];
	Animation* mCurrentAnimation;

	int mSkill1Count; 
	int mSkill2Count;

public:
	Alchemist(int indexX, int indexY, float sizeX, float sizeY);
	void Init() override;
	void Update() override;
	void Release() override;
	void Render(HDC hdc) override;

	void SetAnimation(int listNum) override;
	void BasicAttack();
	void Skill1(); // 역병 플라스크 최대 충전 3
	void Skill2(); // 불지옥 플라스크 최대 충전 3

	void SkulSwitch(int indexX, int indexY) override;
	void SkulReset() override;

};