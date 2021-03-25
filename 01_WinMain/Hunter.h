#pragma once
#pragma once
#pragma once
#include "Player.h"

class Animation;
class Hunter : public Player
{
	Image* mArrow;

	enum class Motion : int {
		rightIdle = 0,
		leftIdle,
		rightWalk,
		leftWalk,
		rightDash,
		leftDash,

		rightAttack1,
		leftAttack1,

		rightSkill1,
		leftSkill1,
		rightSkill2,
		leftSkill2,

		rightSwitching,
		leftSwitching,

		End
	};

	Animation* mAnimationList[(int)Motion::End];
	Animation* mCurrentAnimation;


public:
	Hunter(int indexX, int indexY, float sizeX, float sizeY);
	void Init() override;
	void Update() override;
	void Release() override;
	void Render(HDC hdc) override;

	void SetAnimation(int listNum) override;
	void BasicAttack();
	void Skill1(); // ¸ÖÆ¼¼¦
	void Skill2(); // ÇÇ¾î½º ¼¦

	void SkulSwitch(int indexX, int indexY) override;
	void SkulReset() override;

	void SetAttackSpeed()override;

};