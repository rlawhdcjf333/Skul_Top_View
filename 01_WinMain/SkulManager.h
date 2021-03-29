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
	float mPhysicalAtk;
	float mMagicalAtk;

	bool mInvincibility;

	float mSwitchingCoolTime;
	float mInitSwitchingCoolTime;

	float mHitTime;

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
	int GetLostHpPercentage();
	void PlusGold(int val);
	void MinusGold(int val) { if(mGold-val>=0) mGold -= val;}
	int GetGold() { return mGold; }

	Player* GetCurrentSkul() { return mCurrentSkul; }
	Player* GetAlterSkul() { return mAlterSkul; }
	void SetCurrentSkul(Player* skul) { mCurrentSkul = skul;}

	Player* NewSkulGet(Player* skul);

	void Invincibilize() { mInvincibility = true; }
	void Disinvincibilize() { mInvincibility = false; }

	float GetAtkSpeed() { return mAtkSpeed; }
	void SetAtkSpeed(float val) { mAtkSpeed = val; }

	float GetPhysicalAtk() { return mPhysicalAtk; }
	void SetPhysicalAtk(float val) { mPhysicalAtk = val; }

	float GetMagicalAtk() { return mMagicalAtk; }
	void SetMagicalAtk(float val) { mMagicalAtk = val; }

	float GetHitTime() { return mHitTime; }
	void SetHitTime(float val) { mHitTime = val; }

	Inventory* GetInventory() { return mInventory; }

	void Reset() { if(mCurrentSkul != nullptr)mCurrentSkul->PathReset();}
};
#define SKUL SkulManager::GetInstance()
#define mAttackSpeed SkulManager::GetInstance()->GetAtkSpeed()
#define mPhysicalAttackPower SkulManager::GetInstance()->GetPhysicalAtk()
#define mMagicalAttackPower SkulManager::GetInstance()->GetMagicalAtk()
#define mHit SkulManager::GetInstance()->GetHitTime()
#define mInitPhysicalPower 4.f