#pragma once
#include "Skuls.h"

class Player;
class SkulManager {

	Singleton(SkulManager)
private:
	Player* mCurrentSkul;
	Player* mAlterSkul;

	const int mInitHp = 100;
	int mHp;
	int mMaxHp;
	
	const int mInitGold =0;
	int mGold;

	const float mInitAttackSpeed = 0.1f;
	float mAttackSpeed;
	const float mMaxAttackSpeed = 0.02f;

	const int mMinPhysicalAtk = 1;
	int mPhysicalAtk;

	const int mMinMagicalAtk = 1;
	int mMagicalAtk;

	const int mInitMovingSpeed = 200;
	int mMovingSpeed;

	bool mInvincibility;

	struct Buff
	{
		function <void()> mBuffFunc; //버프 끝나고 실행될 함수
		float mDuration;
	};

	vector <Buff> mBuffList;

public:
	void Init();
	void Damage(int damage) { if(!mInvincibility) mHp -= damage; }
	void Update();
	void ChangeSkul();
	
	void PlusHp(int val); 
	void MinusHp(int val) { mHp -= val; }
	int GetHp() { return mHp; }
	void PlusGold(int val);
	void MinusGold(int val) { if(mGold-val>=0) mGold -= val;}
	int GetGold() { return mGold; }

	void RegBuff(function <void()> func, float duration);

	Player* GetCurrentSkul() { return mCurrentSkul; }
	Player* GetAlterSkul() { return mAlterSkul; }
	void SetCurrentSkul(Player* skul) { mCurrentSkul = skul;}

	Player* NewSkulGet(Player* skul);

	void Invincibilize() { mInvincibility = true; }
	void Disinvincibilize() { mInvincibility = false; }

};
#define SKUL SkulManager::GetInstance()