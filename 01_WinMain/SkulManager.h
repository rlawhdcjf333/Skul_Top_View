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
	float mGoldBonusRatio;

	float mAtkSpeed;
	float mPhysicalAtk;
	float mMagicalAtk;

	float mInitMovingSpeed;
	bool mInvincibility;

	float mSwitchingCoolTime;
	float mInitSwitchingCoolTime;

	float mHitTime;

	int mDamageDecrease;
	stack<int> mDamages;
	Inventory* mInventory;

	Image* mPlayerFrame;
	Image* mTimerFrame;
	Image* mHpBar;
	Image* mCurrentSkulFace;
	Image* mSecondSkulFace;

	wstring name;
	wstring name2;

public:
	void Init();
	void Damage(int damage) { if (!mInvincibility) { damage -= mDamageDecrease; mHp -= damage; mDamages.emplace(damage); } }
	void Update();
	void Release();
	void Render(HDC hdc);

	void ChangeSkul();

	void SetSwitchingCoolTime(float val) { mSwitchingCoolTime = val; }
	float const GetSwitchingCoolTime() { return mSwitchingCoolTime; }
	float const GetInitSwitchingCoolTime() { return mInitSwitchingCoolTime; }
	void PlusHp(int val);
	void MinusHp(int val) { mHp -= val; }
	int GetHp() { return mHp; }
	void SetHp(int val) { mHp = val; }


	int GetMaxHp() { return mMaxHp; }
	void SetMaxHp(int val) { mMaxHp = val; }
	int GetLostHpPercentage();
	void PlusGold(int val);
	void MinusGold(int val) { if(mGold-val>=0) mGold -= val;}
	int GetGold() { return mGold; }
	void SetGoldBonusRatio(float val) { mGoldBonusRatio = val; }
	float GetGoldBonusRatio() { return mGoldBonusRatio; }

	Player* GetCurrentSkul() { return mCurrentSkul; }
	Player* GetAlterSkul() { return mAlterSkul; }
	void SetCurrentSkul(Player* skul) { mCurrentSkul = skul;}

	void NewSkulGet(Player* skul);

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

	float GetInitMovingSpeed() { return mInitMovingSpeed; }
	void SetInitMovingSpeed(float val) { mInitMovingSpeed = val; }

	int GetDamageDecrease() { return mDamageDecrease; }
	void SetDamageDecrease(int val) { mDamageDecrease = val; }

	void Reset() { if(mCurrentSkul != nullptr)mCurrentSkul->PathReset();}
	void SceneInit();
};
#define SKUL SkulManager::GetInstance()
#define mAttackSpeed SkulManager::GetInstance()->GetAtkSpeed()
#define mPhysicalAttackPower SkulManager::GetInstance()->GetPhysicalAtk()
#define mMagicalAttackPower SkulManager::GetInstance()->GetMagicalAtk()
#define mHit SkulManager::GetInstance()->GetHitTime()
#define mInitPhysicalPower 4.f
#define mInitSpeed SkulManager::GetInstance()->GetInitMovingSpeed()