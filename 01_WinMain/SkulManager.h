#pragma once
#include "Skuls.h"

class Condition;
class Player;
class Inventory;
class SkulManager {

	Singleton(SkulManager)
private:
	Player* mCurrentSkul;
	Player* mAlterSkul;

	int mHp;
	int mMaxHp;
	
	int mGold;

	float mAtkSpeed;
	int mPhysicalAtk;
	int mMagicalAtk;

	bool mInvincibility;

	float mSwitchingCoolTime;
	float mInitSwitchingCoolTime;

	Inventory* mInventory;

public:
	void Init();
	void Damage(int damage) { if(!mInvincibility) mHp -= damage; }
	void Update();
	void Release();
	void Render(HDC hdc);
	
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

	float GetAtkSpeed() { return mAtkSpeed; }
	void SetAtkSpeed(float val) { mAtkSpeed = val; }

	int GetPhysicalAtk() { return mPhysicalAtk; }
	void SetPhysicalAtk(int val) { mPhysicalAtk = val; }

	int GetMagicalAtk() { return mMagicalAtk; }
	void SetMagicalAtk(int val) { mMagicalAtk = val; }

	Inventory* GetInventory() { return mInventory; }

};
#define SKUL SkulManager::GetInstance()
#define mAttackSpeed SkulManager::GetInstance()->GetAtkSpeed()
#define mPhysicalAttackPower SkulManager::GetInstance()->GetPhysicalAtk()
#define mMagicalAttackPower SkulManager::GetInstance()->GetMagicalAtk()