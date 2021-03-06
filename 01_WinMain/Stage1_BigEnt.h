#pragma once
#include "Enemy.h"
class Stage1_BigEnt : public Enemy
{
	enum class StateType {
		Attack1 = 0,
		Attack2,
		Dead,
		Idle,
		End
	};
	wstring mStateType[(int)StateType::End] = { L"Attack1",L"Attack2",L"Dead",L"Idle"};
	map<StateType, AnimationPair> mAnimationMap[2];
	StateType mType;
public:
	Stage1_BigEnt(int indexX, int indexY);
	void Init() override;
	void Update() override;
	void Release() override;
	void Render(HDC hdc) override;

	void Walk();
	void Attack();
	void Idle();
	void Hit();
	void Damage(int Damage)override;
	void Move(int speed);
	void AttackDamage(); //공격 모션 데미지 판정
	void ReMove(); //타일에서 밀려났을 때

	void AnimationSet();
	void CurrentSet(StateType type, Direction direction);
	void MoveReset();
	void EnemyInTileCheck(); //타일안 보정 체크
	void KnockBack();
	void KnockBackMove();
	bool AttackCheck(int area); //공격 가능 여부 체크
	bool WalkCheck();
};

