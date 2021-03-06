#include "pch.h"
#include "SkulManager.h"
#include "Player.h"
#include "Effect.h"
#include "Condition.h"
#include "Inventory.h"
#include "Items.h"
#include "FixedSysFont.h"

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

	//스컬ui프레임이미지
	IMAGEMANAGER->LoadFromFile(L"PlayerFrame", Resources(L"/Frame/PlayerFrame.bmp"), 336, 132,true);
	IMAGEMANAGER->LoadFromFile(L"TimerFrame", Resources(L"/Frame/TimerFrame.bmp"), 132, 42, true);
	IMAGEMANAGER->LoadFromFile(L"HpBar", Resources(L"/Frame/HpBar.bmp"), 236, 20, true);

	//스컬ui이미지
	IMAGEMANAGER->LoadFromFile(L"Alchemist1", Resources(L"SkulImage/Alchemist1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"Alchemist2", Resources(L"SkulImage/Alchemist2.bmp"), 42, 42, true);
	IMAGEMANAGER->LoadFromFile(L"Berserker1", Resources(L"SkulImage/Berserker1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"Berserker2", Resources(L"SkulImage/Berserker2.bmp"), 42, 42, true);
	IMAGEMANAGER->LoadFromFile(L"Clown1", Resources(L"SkulImage/Clown1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"Clown2", Resources(L"SkulImage/Clown2.bmp"), 42, 42, true);
	IMAGEMANAGER->LoadFromFile(L"Ent1", Resources(L"SkulImage/Ent1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"Ent2", Resources(L"SkulImage/Ent2.bmp"), 42, 42, true);
	IMAGEMANAGER->LoadFromFile(L"GrimReaper1", Resources(L"SkulImage/GrimReaper1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"GrimReaper2", Resources(L"SkulImage/GrimReaper2.bmp"), 42, 42, true);
	IMAGEMANAGER->LoadFromFile(L"HighWarlock1", Resources(L"SkulImage/HighWarlock1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"HighWarlock2", Resources(L"SkulImage/HighWarlock2.bmp"), 42, 42, true);
	IMAGEMANAGER->LoadFromFile(L"Hunter1", Resources(L"SkulImage/Hunter1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"Hunter2", Resources(L"SkulImage/Hunter2.bmp"), 42, 42, true);
	IMAGEMANAGER->LoadFromFile(L"LittleBone1", Resources(L"SkulImage/LittleBone1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"LittleBone2", Resources(L"SkulImage/LittleBone2.bmp"), 42, 42, true);
	IMAGEMANAGER->LoadFromFile(L"Mino1", Resources(L"SkulImage/Mino1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"Mino2", Resources(L"SkulImage/Mino2.bmp"), 42, 42, true);
	IMAGEMANAGER->LoadFromFile(L"Ninja1", Resources(L"SkulImage/Ninja1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"Ninja2", Resources(L"SkulImage/Ninja2.bmp"), 42, 42, true);
	IMAGEMANAGER->LoadFromFile(L"PettyThief1", Resources(L"SkulImage/PettyThief1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"PettyThief2", Resources(L"SkulImage/PettyThief2.bmp"), 42, 42, true);
	IMAGEMANAGER->LoadFromFile(L"Sword1", Resources(L"SkulImage/Sword1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"Sword2", Resources(L"SkulImage/Sword2.bmp"), 42, 42, true);
	IMAGEMANAGER->LoadFromFile(L"Warrior1", Resources(L"SkulImage/Warrior1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"Warrior2", Resources(L"SkulImage/Warrior2.bmp"), 42, 42, true);
	IMAGEMANAGER->LoadFromFile(L"WereWolf1", Resources(L"SkulImage/WereWolf1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"WereWolf2", Resources(L"SkulImage/WereWolf2.bmp"), 42, 42, true);
	IMAGEMANAGER->LoadFromFile(L"Yaksha1", Resources(L"SkulImage/Yaksha1.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"Yaksha1", Resources(L"SkulImage/Yaksha2.bmp"), 42, 42, true);

	IMAGEMANAGER->LoadFromFile(L"AlchemistSkill1", Resources(L"skillImage/AlchemistSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"AlchemistSkill2", Resources(L"skillImage/AlchemistSkill2.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"BerserkerSkill1", Resources(L"skillImage/BerserkerSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"BerserkerSkill2", Resources(L"skillImage/BerserkerSkill2.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"ClownSkill1", Resources(L"skillImage/ClownSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"ClownSkill2", Resources(L"skillImage/ClownSkill2.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"EntSkill1", Resources(L"skillImage/EntSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"EntSkill2", Resources(L"skillImage/EntSkill2.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"GrimReaperSkill1", Resources(L"skillImage/GrimReaperSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"GrimReaperSkill2", Resources(L"skillImage/GrimReaperSkill2.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"GrimReaperSkill1", Resources(L"skillImage/HighWarlockSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"GrimReaperSkill2", Resources(L"skillImage/HighWarlockSkill2.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"HunterSkill1", Resources(L"skillImage/HunterSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"HunterSkill2", Resources(L"skillImage/HunterSkill2.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"LittleBoneSkill1", Resources(L"skillImage/LittleBone1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"LittleBoneSkill2", Resources(L"skillImage/LittleBone2.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"MinoSkill1", Resources(L"skillImage/MinoSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"MinoSkill2", Resources(L"skillImage/MinoSkill2.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"NinjaSkill1", Resources(L"skillImage/NinjaSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"NinjaSkill2", Resources(L"skillImage/NinjaSkill2.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"PettyThiefSkill1", Resources(L"skillImage/PettyThiefSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"PettyThiefSkill2", Resources(L"skillImage/PettyThiefSkill2.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"SwordSkill1", Resources(L"skillImage/SwordSkill2.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"SwordSkill2", Resources(L"skillImage/SwordSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"WarriorSkill1", Resources(L"skillImage/WarriorSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"WarriorSkill2", Resources(L"skillImage/WarriorSkill2.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"WerewolfSkill1", Resources(L"skillImage/WereWolfSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"WerewolfSkill2", Resources(L"skillImage/WereWolfSkill2.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"YakshaSkill1", Resources(L"skillImage/YakshaSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"YakshaSkill2", Resources(L"skillImage/YakshaSkill2.bmp"), 24, 24, true);
	
	mPlayerFrame = IMAGEMANAGER->FindImage(L"PlayerFrame");
	mTimerFrame = IMAGEMANAGER->FindImage(L"TimerFrame");
	mHpBar = IMAGEMANAGER->FindImage(L"HpBar");

	mInitSwitchingCoolTime = 8; //교대 쿨 8초
	mSwitchingCoolTime = 0;

	mCurrentSkul = nullptr;
	mAlterSkul= nullptr;

	mMaxHp =  mHp = 100;
	mGold = 0;

	mAtkSpeed = 0.1f;
	mPhysicalAtk = 4;
	mMagicalAtk = 4;
	mInitMovingSpeed = 200;

	mInvincibility = false;
	mHitTime = 0;

	mInventory = nullptr;

	mCurrentSkulFace = IMAGEMANAGER->FindImage(L"LittleBone1");
	mSkill1 = IMAGEMANAGER->FindImage(L"LittleBoneSkill1");
	mSkill2 = IMAGEMANAGER->FindImage(L"LittleBoneSkill2");
}


void SkulManager::Update() 
{

	if (mHp <= 0) // 죽었을 때 할 일;
	{
		//GameEventManager::PushEvent() 대충 이런거 하나 하고
		SoundPlayer::GetInstance()->Stop(L"Title");
		SoundPlayer::GetInstance()->Stop(L"Stage1");
		SoundPlayer::GetInstance()->Stop(L"Title");
		SoundPlayer::GetInstance()->Stop(L"Stage2");
		SoundPlayer::GetInstance()->Stop(L"Store");
		SoundPlayer::GetInstance()->Stop(L"Main");
		SoundPlayer::GetInstance()->Stop(L"Airman");

		Release();
		SceneManager::GetInstance()->LoadScene(L"GameScene");
	}
	else
	{
		
		if (INPUT->GetKeyDown(VK_TAB)) //인벤토리 활성
		{
			mInventory->SetIsToggle(!mInventory->GetIsToggle());
		}

		if (mInventory->GetIsToggle())
		{
			mInventory->Update(); // 인벤토리 활성 중일때 인벤토리 업데이트 == 커서가 위치한 곳에 따라 렌더 바꾸기
		}

		mInventory->ActivateItemList();

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

		if (mAlterSkul) //교대 스컬 쿨타임 돌리기
		{
			mAlterSkul->Skill1();
			mAlterSkul->Skill2();
		}
	}

	name = mCurrentSkul->GetKeyName();
	mCurrentSkulFace = IMAGEMANAGER->FindImage(name.append(L"1"));

	if(mAlterSkul)
		name2 = mAlterSkul->GetKeyName();
	mSecondSkulFace = IMAGEMANAGER->FindImage(name2.append(L"2"));

	name = mCurrentSkul->GetKeyName();
	mSkill1= IMAGEMANAGER->FindImage(name.append(L"Skill1"));
	name = mCurrentSkul->GetKeyName();
	mSkill2 = IMAGEMANAGER->FindImage(name.append(L"Skill2"));
}

void SkulManager::Release()
{
	if (mCurrentSkul) { mCurrentSkul->Release(); SafeDelete(mCurrentSkul);} //이제 게임 씬에서 더이상 플레이어 스컬을 삭제하지 않으므로 얘가 해야 됨
	if (mAlterSkul) { mAlterSkul->Release(); SafeDelete(mAlterSkul);}
	mInventory->Release(); //인벤토리도 날려주자
	SafeDelete(mInventory);
}

void SkulManager::Render(HDC hdc)
{
	mPlayerFrame->Render(hdc, 0, 588);
	mTimerFrame->Render(hdc, 0, 0);

	mHpBar->ScaleRender(hdc, 86, 676, 236 * (float)((float)mHp / (float)mMaxHp), 20);
	if(mCurrentSkulFace)
	mCurrentSkulFace->Render(hdc, 10, 588);

	if (mSecondSkulFace)
	mSecondSkulFace->Render(hdc, 20, 650);
	
	if (mSkill1 && mSkill2)
	{
		mSkill1->ScaleRender(hdc, 116, 618, 48, 48);
		mSkill2->ScaleRender(hdc, 186, 618, 48, 48);
	}

	while (mDamages.size() > 0) {
		new FixedSysFont(mCurrentSkul->GetX(),mCurrentSkul->GetY(), 100, 100, to_wstring(mDamages.top()), FontColor::Red);
		mDamages.pop();
	}
	mInventory->Render(hdc);
	//TextOut(hdc, 200, 100, to_wstring(mPhysicalAtk).c_str(), to_wstring(mPhysicalAtk).size());

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));
	TextOut(hdc, 220, 678, to_wstring(mMaxHp).c_str(), to_wstring(mMaxHp).size());
	TextOut(hdc, 200, 678, L"/", 1);
	TextOut(hdc, 160, 678, to_wstring(mHp).c_str(), to_wstring(mHp).size());
	TextOut(hdc, 38, 12, to_wstring((int)Time::GetInstance()->GetWorldTime()/60).c_str(), to_wstring((int)Time::GetInstance()->GetWorldTime() / 60).size());
	TextOut(hdc, 63, 12, L":", 1);
	TextOut(hdc, 88, 12, to_wstring((int)Time::GetInstance()->GetWorldTime() % 60).c_str(), to_wstring((int)Time::GetInstance()->GetWorldTime() % 60).size());
	TextOut(hdc, 1000, 670, to_wstring(mGold).c_str(), to_wstring(mGold).size());
	SetBkMode(hdc, OPAQUE);
	SetTextColor(hdc, RGB(0, 0, 0));
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
	float bonus = (float)val * mGoldBonusRatio/100.f;
	if (bonus > 0 and bonus<1) bonus = 1;
	mGold += (val + (int)bonus);
	(new Effect(L"GoldGet", mCurrentSkul->GetX(), mCurrentSkul->GetY()-50, EffectType::Normal))->Scaling(50, 50);
}

void SkulManager::NewSkulGet(Player* skul)
{
	if (mAlterSkul) {
		Player* temp = mCurrentSkul;
		mCurrentSkul = skul;
		mCurrentSkul->SetIsActive(true);
		mCurrentSkul->SetAnimation(0);
		CAMERA->SetTarget(mCurrentSkul);
		Obj->DeleteSkul(temp);
	}
	else
	{
		mAlterSkul = skul;
		mCurrentSkul->SkulReset();
		mCurrentSkul->SetIsActive(false);
		Player* tmp = mCurrentSkul;
		mCurrentSkul = mAlterSkul;
		mAlterSkul = tmp;
		mCurrentSkul->SetIsActive(true);
		mCurrentSkul->SetAnimation(0);
	}
}

void SkulManager::SceneInit()
{
	mInitSwitchingCoolTime = 8; //교대 쿨 8초
	mSwitchingCoolTime = 0;

	mCurrentSkul = new LittleBone(41, 57, 30, 30);
	mAlterSkul = nullptr;

	mMaxHp = mHp = 100;
	mGold = 0;

	mAtkSpeed = 0.1f;
	mPhysicalAtk = 4;
	mMagicalAtk = 4;
	mInitMovingSpeed = 200;

	mInvincibility = false;
	mHitTime = 0;

	mInventory = new Inventory();
	mInventory->GetSkul(new LittleBoneHead(0, 0));
	mInventory->GetFirstSkul()->SetIsTrashed(false);
}

int SkulManager::GetLostHpPercentage()
{
	return (float)100 * (mMaxHp - mHp) / (float)mMaxHp;
}