#include "pch.h"
#include "SkulManager.h"
#include "Player.h"
#include "Effect.h"
#include "Condition.h"
#include "Inventory.h"

void SkulManager::Init()
{
	IMAGEMANAGER->LoadFromFile(L"SkulHitEffect", Resources(L"/skul/hit.bmp"), 1100, 100, 11, 1, true);
	IMAGEMANAGER->LoadFromFile(L"DashRightEffect", Resources(L"/skul/dashRight.bmp"), 2400, 100, 24, 1, true);
	IMAGEMANAGER->LoadFromFile(L"DashLeftEffect", Resources(L"/skul/dashLeft.bmp"), 2400, 100, 24, 1, true);
	IMAGEMANAGER->LoadFromFile(L"NinjaHit", Resources(L"skul/ninjaHit.bmp"), 700, 200, 7, 2, true);
	IMAGEMANAGER->LoadFromFile(L"GoldGet", Resources(L"/skul/goldGet.bmp"), 500, 500, 5, 5, true);
	IMAGEMANAGER->LoadFromFile(L"Bleeding", Resources(L"/skul/bleeding.bmp"), 900, 100, 9, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Burning", Resources(L"/skul/burning.bmp"), 400, 300, 4, 3, true);
	IMAGEMANAGER->LoadFromFile(L"Healing", Resources(L"/skul/healing.bmp"), 600, 200, 6, 2, true);

	mInitSwitchingCoolTime = 8; //���� �� 8��
	mSwitchingCoolTime = 0;

	mCurrentSkul = nullptr;
	mAlterSkul= nullptr;

	mMaxHp =  mHp = 100;
	mGold = 0;

	mAtkSpeed = 0.1f;
	mPhysicalAtk = 4;
	mMagicalAtk = 4;

	mInvincibility = false;
	mHitTime = 0;

	mInventory = new Inventory();
}


void SkulManager::Update() 
{

	if (mHp <= 0) // �׾��� �� �� ��;
	{
		//GameEventManager::PushEvent() ���� �̷��� �ϳ� �ϰ�
		//Init(); //���� �Ŵ��� �ʱ�ȭ
		//SceneManager::GetInstance()->LoadScene(L"GameScene");
	}
	else
	{
		if (INPUT->GetKeyDown(VK_TAB)) //�κ��丮 Ȱ��
		{
			mInventory->SetIsToggle(!mInventory->GetIsToggle());
		}

		if (mInventory->GetIsToggle())
		{
			mInventory->Update(); // �κ��丮 Ȱ�� ���϶� �κ��丮 ������Ʈ == Ŀ���� ��ġ�� ���� ���� ���� �ٲٱ�
		}

		mHitTime -= dTime;
		if (mHitTime < 0) mHitTime = 0;

		mSwitchingCoolTime -= dTime;
		if (mSwitchingCoolTime < 0) mSwitchingCoolTime = 0;
		if (INPUT->GetKeyDown(VK_SPACE) and mSwitchingCoolTime==0)
		{
			mSwitchingCoolTime = mInitSwitchingCoolTime;
			ChangeSkul();
			CAMERA->SetTarget(mCurrentSkul);
		}

		if (mAlterSkul) //���� ���� ��Ÿ�� ������
		{
			mAlterSkul->Skill1();
			mAlterSkul->Skill2();
		}
	}
}

void SkulManager::Release()
{
	if (mCurrentSkul) { mCurrentSkul->Release(); SafeDelete(mCurrentSkul); } //���� ���� ������ ���̻� �÷��̾� ������ �������� �����Ƿ� �갡 �ؾ� ��
	if (mAlterSkul) { mAlterSkul->Release(); SafeDelete(mAlterSkul); }
	mInventory->Release(); //�κ��丮�� ��������
	SafeDelete(mInventory);
}

void SkulManager::Render(HDC hdc)
{
	mInventory->Render(hdc);
	TextOut(hdc, 200, 100, to_wstring(mHp).c_str(), to_wstring(mHp).size());
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

void SkulManager::PlusHp(int val)
{
	if (mMaxHp >= mHp + val)
	{
		mHp += val; 
		(new Effect(L"Healing", mCurrentSkul->GetX(), mCurrentSkul->GetY() - 25, EffectType::Normal))->Scaling(100, 100, 0.1f);
	}
}

void SkulManager::PlusGold(int val)
{ 
	mGold += val; 
	(new Effect(L"GoldGet", mCurrentSkul->GetX(), mCurrentSkul->GetY()-50, EffectType::Normal))->Scaling(50, 50);
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

int SkulManager::GetLostHpPercentage()
{
	return (float)100 * (mMaxHp - mHp) / (float)mMaxHp;
}