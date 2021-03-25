#pragma once
#include "Skuls.h"

class Player;
class SkulManager {

	Singleton(SkulManager)
private:
	Player* mCurrentSkul;
	Player* mAlterSkul;
	int mHp;
	int mGold;
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
	void PlusHp(int val) { mHp += val; }
	void MinusHp(int val) { mHp -= val; }
	int GetHp() { return mHp; }
	void PlusGold(int val) { mGold += val; }
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