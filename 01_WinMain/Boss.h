#pragma once
#include "GameObject.h"
class Boss : public GameObject
{
	enum class BossPattern : int {
		StartRanding,
		Idle,
		Move,
		Randing,
		PhaseUp,
		Normal,
		SideCut,
		RandingAttack,
		WaveAttack,
		ChargingAttack,
		DoubleWaveAttack,
		Dead,
		End
	};
	BossPattern mCurentPattern;
	Animation* mAnimation;
	Image* mImage;
	bool mNowStartEvent;
	bool mNowPatternPlay;
	bool mInvincibility;
	bool mDeadCheck;
	int mHp;
	int mCurentImage;
	int mFrameIndexX;
	int mFrameIndexY;
	float mAngle;
	Tile* mTargetTile;
	queue<int> mDamages;
	int mNextAnimation;
	vector<Tile*> mPath;
	float mSkillTime1;
	float mSkillTime2;
	bool mShotCheck;
public :
	Boss(int x, int y);
	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();
	void Idle();

	void StartRanding();
	void Randing(); //���� �� ���� �̺�Ʈ
	void Normal(); // �⺻ ���� �� ��ü ����
	void SideCut();
	void RandingAttack();
	void ChargingAttack();
	void Dead();
	
	//2������ �߰� ���
	void PhaseUp();//������ �� ���� �̺�Ʈ
	void WaveAttack();
	void DoubleWaveAttack();

	void Move();
	void Damage(int damage);

	bool GetDeadCheck() { return mDeadCheck; }
};

