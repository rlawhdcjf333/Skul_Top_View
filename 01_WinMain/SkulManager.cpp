#include "pch.h"
#include "SkulManager.h"
#include "Player.h"

void SkulManager::Init()
{
	IMAGEMANAGER->LoadFromFile(L"SkulHitEffect", Resources(L"/skul/hit.bmp"), 1100, 100, 11, 1, true);
	IMAGEMANAGER->LoadFromFile(L"DashRightEffect", Resources(L"/skul/dashRight.bmp"), 2400, 100, 24, 1, true);
	IMAGEMANAGER->LoadFromFile(L"DashLeftEffect", Resources(L"/skul/dashLeft.bmp"), 2400, 100, 24, 1, true);
	IMAGEMANAGER->LoadFromFile(L"NinjaHit", Resources(L"skul/ninjaHit.bmp"), 700, 200, 7, 2, true);

	mCurrentSkul = nullptr;
	mAlterSkul= nullptr;

	mHp = 100;
	mGold = 0;
	mInvincibility = false;
	mBuffList = {};
}


void SkulManager::Update() {
	if (INPUT->GetKeyDown(VK_SPACE))
	{
		ChangeSkul();
		CAMERA->SetTarget(mCurrentSkul);
	}

	for (int i = 0; i < mBuffList.size(); i++)  //버프 관리
	{
		mBuffList[i].mDuration -= dTime;
		if (mBuffList[i].mDuration <= 0)
		{
			mBuffList[i].mBuffFunc();
			mBuffList.erase(mBuffList.begin() + i);
			i--;
		}
	}

	if (mHp <= 0) // 죽었을 때 할 일;
	{


	}

	if (mAlterSkul) //교대 스컬 쿨타임 돌리기
	{
		mAlterSkul->Skill1();
		mAlterSkul->Skill2();
	}

	

}

void SkulManager::ChangeSkul()
{
	if (mAlterSkul)
	{
		mCurrentSkul->SkulReset();
		mCurrentSkul->SetIsActive(false);
		Player* tmp = mCurrentSkul;
		mCurrentSkul = mAlterSkul;
		mAlterSkul = tmp;
		mCurrentSkul->SetIsActive(true);
		mCurrentSkul->SkulSwitch(mAlterSkul->GetIndexX(), mAlterSkul->GetIndexY());
	}
}


void SkulManager::RegBuff(function<void()> func, float duration)
{
	Buff tmp;
	tmp.mBuffFunc = func;
	tmp.mDuration = duration;
	mBuffList.push_back(tmp);
}

Player* SkulManager::NewSkulGet(Player* skul)
{
	for (Buff elem : mBuffList) //새로운 스컬을 얻을 경우 모든 버프 초기화 ->버그방지
	{
		elem.mBuffFunc();
	}
	mBuffList.clear();


	if (mAlterSkul) {
		Player* temp = mCurrentSkul;
		mCurrentSkul = skul;
		return temp;
	}
	mAlterSkul = mCurrentSkul;
	mCurrentSkul = skul;
	return nullptr;
}
