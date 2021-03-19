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
public:
	void Update();
	void ChangeSkul();
	void SetHp() {}
	void GetHp() {}

	Player* GetCurrentSkul() { return mCurrentSkul; }
	void SetCurrentSkul(Player* skul) { mCurrentSkul = skul; }

	Player* NewSkulGet(Player* skul);
};
#define SKUL SkulManager::GetInstance()