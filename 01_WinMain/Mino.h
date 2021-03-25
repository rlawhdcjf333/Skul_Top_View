#pragma once
#pragma once
#pragma once
#include "Player.h"

class Animation;
class Mino : public Player
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
		rightSkill2CancelAttack,
		leftSkill2CancelAttack,

		rightSwitching,
		leftSwitching,

		End
	};

	Animation* mAnimationList[(int)Motion::End];
	Animation* mCurrentAnimation;


public:
	Mino(int indexX, int indexY, float sizeX, float sizeY);
	void Init() override;
	void Update() override;
	void Release() override;
	void Render(HDC hdc) override;

	void SetAnimation(int listNum) override;
	void BasicAttack();
	void Skill1(); //돌진 박치기
	void Skill2(); //돌진 둔기강타

	void SkulSwitch(int indexX, int indexY) override;
	void SkulReset() override;

	void SetAttackSpeed()override;

};