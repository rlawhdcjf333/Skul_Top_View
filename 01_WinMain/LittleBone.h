#pragma once
#include "Player.h"
class Animation;
class LittleBone :public Player
{
	Image* mHeadImage;

	enum class Motion : int {
		rightIdle=0,
		leftIdle,
		rightWalk,
		leftWalk,
		rightDash,
		leftDash,
		rightAttack1,
		rightAttack2,
		leftAttack1,
		leftAttack2,

		switchAttack,
		rightSkill1,
		leftSkill1,

		rightIdleHeadless,
		leftIdleHeadless,
		rightWalkHeadless,
		leftWalkHeadless,
		rightDashHeadless,
		leftDashHeadless,
		rightAttack1Headless,
		rightAttack2Headless,
		leftAttack1Headless,
		leftAttack2Headless,

		respawning,

		End
	};

	Animation* mAnimationList[(int)Motion::End];
	Animation* mCurrentAnimation;

	int mRotationCount;
	bool mIsHead;
	float mSkill1CoolTime;

public:
	LittleBone(int indexX, int indexY, float sizeX, float sizeY);
	void Init() override;
	void Update() override;
	void Release() override;
	void Render(HDC hdc) override;
	
	void SetAnimation(int listNum) override;

	void SkulSwitch(int indexX, int indexY) override;
	void SkulReset() override;

	void BasicAttack();
	void Skill1();
	void Skill2() {};

	void SetAttackSpeed()override;

};

