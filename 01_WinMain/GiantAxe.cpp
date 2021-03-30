#include "pch.h"
#include "GiantAxe.h"
#include "Inventory.h"
#include "Effect.h"

GiantAxe::GiantAxe(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "GiantAxe";

	mItemName = L"거인의 도끼";
	mExplanation = L"당신이 누구건 그들의 매머드를 건드리지 마세요.";
	mEffect = L"스컬 교대 시 주변 적들을 밀쳐내고 물리데미지를 입힙니다.\n물리공격력이 35 % 증가합니다.";

	IMAGEMANAGER->LoadFromFile(L"March", Resources(L"/item/March.bmp"), 78, 78, true);
	mSlot1Name = L"행군";
	mSlot1Image = IMAGEMANAGER->FindImage(L"March");
	mSlot1Explanation = L"물리공격력이 15/25/40/100% 증폭됩니다.";

	IMAGEMANAGER->LoadFromFile(L"GiantAxe", Resources(L"item/GiantAxe1.bmp"), 26, 26, true);
	mImage = IMAGEMANAGER->FindImage(L"GiantAxe");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mType = ItemType::CommonItem;

	mValue = (float)mPhysicalAttackPower*35.f/100.f;

	mActivationFunc = []() 
	{
		if (SKUL->GetSwitchingCoolTime() == SKUL->GetInitSwitchingCoolTime() and INPUT->GetKeyDown(VK_SPACE))
		{
			SKUL->GetCurrentSkul()->Attack(mPhysicalAttackPower, 2, AttackType::Whirlwind);
		}
	};
	mDeactivationFunc = [this]() {SKUL->SetPhysicalAtk(mPhysicalAttackPower - mValue);};
	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void GiantAxe::Update()
{
	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetRect();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //획득 트리거
		{
			SKUL->GetInventory()->GetItem(this);
			SKUL->SetPhysicalAtk(mPhysicalAttackPower + mValue);
			mIsTrashed = false;
			SetObjectOnTile(0, 0); //안보이는 어디론가로 숨겨놓는다... 이러면 어차피 클리핑되서 렌더도 안 돈다;
			mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
		}

		if (INPUT->GetKey('F') and mDuration >= 0)
		{
			mDuration -= dTime;

			SetObjectOnTile(mIndexX, mIndexY);
			mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
			mX += (2 - mDuration) * 2 * cosf(RAND->RandomInt(360) * PI / 180);
			mY -= (2 - mDuration) * 2 * sinf(RAND->RandomInt(360) * PI / 180);
			mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
		}

	}
	else
	{
		mDuration = 2;
		mIsCollision = false;
	}
	if (INPUT->GetKeyUp('F'))
	{
		mDuration = 2;
	}

	if (mDuration < 0) //아이템 파괴 트리거
	{
		new Effect(L"Spark", mX, mY, EffectType::Normal);
		SetObjectOnTile(1, 1); // 적당히 어딘가 쳐박아 두기
		mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	}
}

void GiantAxe::Render(HDC hdc)
{
	CAMERA->ScaleRender(hdc, mImage, mRect.left, mRect.top, mSizeX, mSizeY);

	if (mIsCollision)
	{
		CallFont(hdc, 15, [&]()
		{
			TextOut(hdc, mRect.left - CAMERA->GetRect().left, mRect.top - 40 - CAMERA->GetRect().top, mItemName.c_str(), mItemName.size());
			TextOut(hdc, mRect.left - CAMERA->GetRect().left, mRect.top - 25 - CAMERA->GetRect().top, mExplanation.c_str(), mExplanation.size());
			TextOut(hdc, mRect.left - CAMERA->GetRect().left, mRect.top - 10 - CAMERA->GetRect().top, mEffect.c_str(), mEffect.size());
		});

	}

}

void GiantAxe::Release()
{

}
