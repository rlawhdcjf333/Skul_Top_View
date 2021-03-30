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
	void Randing(); //등장 신 강제 이벤트
	void Normal(); // 기본 공격 불 구체 공격
	void SideCut();
	void RandingAttack();
	void ChargingAttack();
	void Dead();
	
	//2페이지 추가 기술
	void PhaseUp();//페이즈 업 강제 이벤트
	void WaveAttack();
	void DoubleWaveAttack();

	void Move();
	void Damage(int damage);

	bool GetDeadCheck() { return mDeadCheck; }
};

