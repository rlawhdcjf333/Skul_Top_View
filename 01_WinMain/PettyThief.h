#pragma once
#pragma once
#include "Player.h"

class Animation;
class PettyThief: public Player
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
		leftAttack1,
		leftAttack2,

		rightSwitching,
		leftSwitching,
		rightSkill1,
		leftSkill1,
		rightSkill2,
		leftSkill2,

		End
	};

	Animation* mAnimationList[(int)Motion::End];
	Animation* mCurrentAnimation;


public:
	PettyThief(int indexX, int indexY, float sizeX, float sizeY);
	void Init() override;
	void Update() override;
	void Release() override;
	void Render(HDC hdc) override;

	void SetAnimation(int listNum) override;
	void BasicAttack();
	void Skill1();
	void Skill2();

	void SkulSwitch(int indexX, int indexY) override;
	void SkulReset() override;

	void SetAttackSpeed()override;

};