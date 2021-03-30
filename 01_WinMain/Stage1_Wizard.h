#pragma once
#include "Enemy.h"
class Stage1_Wizard : public Enemy
{
	enum class StateType {
		Attack = 0,
		Idle,
		Teleport_In,
		Teleport_Out,
		Hit,
		End
	};
	wstring mStateType[(int)StateType::End] = { L"Attack",L"Idle",L"Teleport_In",L"Teleport_Out",L"Hit" };
	map<StateType, AnimationPair> mAnimationMap[2];
	StateType mType;
	bool mNowTeleport;
	RECT mAttackTarget;
public:
	Stage1_Wizard(int indexX, int indexY);
	void Init() override;
	void Update() override;
	void Release() override;
	void Render(HDC hdc) override;

	void Attack();
	void Idle();
	void Hit();
	void Teleport();
	void Damage(int Damage)override;
	void AttackDamage(); //공격 모션 데미지 판정

	void AnimationSet();
	void CurrentSet(StateType type, Direction direction);
	void MoveReset();
	void EnemyInTileCheck(); //타일안 보정 체크
	void KnockBack();
	void KnockBackMove();
	bool TeleportCheck();
	bool AttackCheck(int area); //공격 가능 여부 체크
};

