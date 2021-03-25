#pragma once
#include "Player.h"

class Animation;
class Ent : public Player
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

		rightCharging,
		leftCharging,

		rightSkill2,
		leftSkill2,
		rightSkill2Full,
		leftSkill2Full,

		End
	};

	Animation* mAnimationList[(int)Motion::End];
	Animation* mCurrentAnimation;


public:
	Ent(int indexX, int indexY, float sizeX, float sizeY);
	void Init() override;
	void Update() override;
	void Release() override;
	void Render(HDC hdc) override;

	void SetAnimation(int listNum) override;
	void BasicAttack();
	void Skill1(); //µ¹Áø
	void Skill2(); //Â÷Â¡

	void SkulSwitch(int indexX, int indexY) override;
	void SkulReset() override;

	void SwitchAttack();

	void SetAttackSpeed()override;
};