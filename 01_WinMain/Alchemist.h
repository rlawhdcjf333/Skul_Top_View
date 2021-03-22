#pragma once
#include "Player.h"

class Animation;
class Alchemist : public Player
{
	Image* mProjectile;
	Image* mFlask1;
	Image* mFlask2;

	GameObject* mAttackTarget;
	float mMinDistance;
	float mAttackCoolTime;

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

	float mSkill1CoolTime;
	float mSkill2CoolTime;

public:
	Alchemist(int indexX, int indexY, float sizeX, float sizeY);
	void Init() override;
	void Update() override;
	void Release() override;
	void Render(HDC hdc) override;

	void SetAnimation(int listNum) override;
	void BasicAttack();
	void Skill1(); // ���� �ö�ũ
	void Skill2(); // ������ �ö�ũ

	void SkulSwitch(int indexX, int indexY) override;
	void SkulReset() override;

};