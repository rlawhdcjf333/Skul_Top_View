#pragma once
#include "Enemy.h"
class Stage1_PinkEnt : public Enemy
{
	enum class StateType {
		Attack_Ready = 0,
		Attack,
		Idle,
		Walk,
		Hit,
		End
	};
	wstring mStateType[(int)StateType::End] = { L"Attack_Ready",L"Attack",L"Idle",L"Walk",L"Hit" };
	map<StateType, AnimationPair> mAnimationMap[2];
	StateType mType;
	RECT mGasArea;
public:
	Stage1_PinkEnt(int indexX, int indexY);
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

