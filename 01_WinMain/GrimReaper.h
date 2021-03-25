#pragma once
#include "Player.h"

class GrimReaper :public Player
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

		rightSkill1,
		leftSkill1,
		rightSkill2,
		leftSkill2,

		leftSwitching,
		rightSwitching,

		End
	};

	Animation* mAnimationList[(int)Motion::End];
	Animation* mCurrentAnimation;


public:
	GrimReaper(int indexX, int indexY, float sizeX, float sizeY);
	void Init() override;
	void Update() override;
	void Release() override;
	void Render(HDC hdc) override;

	void SetAnimation(int listNum) override;

	void SkulSwitch(int indexX, int indexY) override;
	void SkulReset() override;

	void BasicAttack();
	void Skill1(); //선고
	void Skill2(); //수확
	void SwitchAttack();

	void SetAttackSpeed()override;



};