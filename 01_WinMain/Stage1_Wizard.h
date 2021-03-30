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
	void AttackDamage(); //���� ��� ������ ����

	void AnimationSet();
	void CurrentSet(StateType type, Direction direction);
	void MoveReset();
	void EnemyInTileCheck(); //Ÿ�Ͼ� ���� üũ
	void KnockBack();
	void KnockBackMove();
	bool TeleportCheck();
	bool AttackCheck(int area); //���� ���� ���� üũ
};

