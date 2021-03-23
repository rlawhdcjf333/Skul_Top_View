#pragma once
#include "Player.h"

class Animation;
class HighWarlock: public Player
{
	Image* mWarlockProjectile;
	Image* mWarlockBarricade;
	Image* mMeteorComp;

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
	HighWarlock(int indexX, int indexY, float sizeX, float sizeY);
	void Init() override;
	void Update() override;
	void Release() override;
	void Render(HDC hdc) override;

	void SetAnimation(int listNum) override;
	void BasicAttack();
	void Skill1(); //메테오
	void Skill2(); //어비스 오브

	void SkulSwitch(int indexX, int indexY) override;
	void SkulReset() override;

	void SetAttackSpeed()override;
};