#include "pch.h"
#include "SkulManager.h"
#include "Player.h"

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
	if (mAlterSkul) {
		Player* temp = mCurrentSkul;
		mCurrentSkul = skul;
		return temp;
	}
	mAlterSkul = mCurrentSkul;
	mCurrentSkul = skul;
	return nullptr;
}
