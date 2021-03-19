#include "pch.h"
#include "SkulManager.h"
#include "Player.h"

void SkulManager::Update() {
	if (INPUT->GetKeyDown(VK_SPACE))
	{
		ChangeSkul();
		CAMERA->SetTarget(mCurrentSkul);
	}
}

void SkulManager::ChangeSkul()
{
	if (mAlterSkul)
	{
		mCurrentSkul->SetIsActive(false);
		Player* tmp = mCurrentSkul;
		mCurrentSkul = mAlterSkul;
		mAlterSkul = tmp;
		mCurrentSkul->SetIsActive(true);
		mCurrentSkul->SkulSwitch(mAlterSkul->GetIndexX(), mAlterSkul->GetIndexY());
	}
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
