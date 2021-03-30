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
	void AttackDamage(); //���� ��� ������ ����
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

