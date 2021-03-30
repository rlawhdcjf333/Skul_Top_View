#pragma once
#include "Enemy.h"
class Stage1_Hammer : public Enemy
{
	enum class StateType {
		Attack = 0,
		Idle,
		Walk,
		Tackle,
		End
	};
	wstring mStateType[(int)StateType::End] = { L"Attack",L"Idle",L"Walk",L"Tackle" };
	map<StateType, AnimationPair> mAnimationMap[2];
	StateType mType;
	float mNextAttackTime;
public:
	Stage1_Hammer(int indexX, int indexY);
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
	void AttackDamage(int range, int damage); //���� ��� ������ ����
	void ReMove(); //Ÿ�Ͽ��� �з����� ��

	void AnimationSet();
	void CurrentSet(StateType type, Direction direction);
	void MoveReset();
	void EnemyInTileCheck(); //Ÿ�Ͼ� ���� üũ
	void KnockBack();
	void KnockBackMove();
	bool AttackCheck(int area); //���� ���� ���� üũ
	bool WalkCheck();
};

