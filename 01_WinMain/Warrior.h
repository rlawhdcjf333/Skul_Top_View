#pragma once
#pragma once
#include "Player.h"

class Animation;
class Warrior : public Player
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
	Warrior(int indexX, int indexY, float sizeX, float sizeY);
	void Init() override;
	void Update() override;
	void Release() override;
	void Render(HDC hdc) override;

	void SetAnimation(int listNum) override;
	void BasicAttack();
	void Skill1(); //¶óÀÌÂ¡ ÄÆ
	void Skill2(); //Âî¸£°í ³Ñ±â±â

	void SkulSwitch(int indexX, int indexY) override;
	void SkulReset() override;

	void SetAttackSpeed()override;


};