#pragma once
#include "Player.h"

class Yaksha :public Player
{
	enum class Motion : int {
		rightIdle = 0,
		leftIdle,
		rightWalk,
		leftWalk,
		rightDash,
		leftDash,
		rightAttack1,
		rightAttack2,
		rightAttack3,
		leftAttack1,
		leftAttack2,
		leftAttack3,

		rightCharging,
		leftCharging,
		rightSkill1,
		leftSkill1,
		rightSkill1Full,
		leftSkill1Full,


		rightSkill2,
		leftSkill2,

		leftSwitching,
		rightSwitching,

		End
	};

	Animation* mAnimationList[(int)Motion::End];
	Animation* mCurrentAnimation;

	int mStompCount;

	float mSkill1CoolTime;
	float mSkill2CoolTime;

public:
	Yaksha(int indexX, int indexY, float sizeX, float sizeY);
	void Init() override;
	void Update() override;
	void Release() override;
	void Render(HDC hdc) override;

	void SetAnimation(int listNum) override;

	void SkulSwitch(int indexX, int indexY) override;
	void SkulReset() override;

	void BasicAttack();
	void Skill1(); //야차 정권
	void Skill2(); //야차 행진
	void SwitchAttack();

	void SetAttackSpeed()override;



};