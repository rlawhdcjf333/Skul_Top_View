#pragma once
#include "Player.h"

class Animation;
class Sword :public Player
{
	Image* mHeadImage;

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

	float mSkill1CoolTime;
	float mSkill2CoolTime;

public:
	Sword(int indexX, int indexY, float sizeX, float sizeY);
	void Init() override;
	void Update() override;
	void Release() override;
	void Render(HDC hdc) override;

	void SetAnimation(int listNum) override;
	void BasicAttack();
	void Skill1(); //µ¹Áø Âî¸£±â
	void Skill2(); //¼¼¹ø Âî¸£±â

	void SkulSwitch(int indexX, int indexY) override;
	void SkulReset() override;

	void SetAttackSpeed()override;

};

