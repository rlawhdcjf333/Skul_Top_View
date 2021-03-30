#include "pch.h"
#include "RageClaw.h"
#include "Inventory.h"
#include "Effect.h"

RageClaw::RageClaw(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "RageClaw";

	mItemName = L"분노의 갈퀴손";
	mExplanation = L"왜 이걸 도축용으로 사용하고 있는거지?";
	mEffect = L"적 처치 시 3초간 공격속도가 55% 증가합니다.";

	IMAGEMANAGER->LoadFromFile(L"Madness", Resources(L"/item/Madness.bmp"), 78, 78, true);
	mSlot1Name = L"광기잇";
	mSlot1Image = IMAGEMANAGER->FindImage(L"Madness");
	mSlot1Explanation = L"적에게 기본공격으로 데미지를 입힐 시 5초간 공격속도가 5%씩 최대 25/45/75/125%까지 증가합니다.";

	IMAGEMANAGER->LoadFromFile(L"RageClaw", Resources(L"item/RageClaw1.bmp"), 24, 24, true);
	mImage = IMAGEMANAGER->FindImage(L"RageClaw");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mType = ItemType::CommonItem;

	mValue = mPhysicalAttackPower / 4.f;

	mActivationFunc = []() {};
	mDeactivationFunc = [this]() {SKUL->SetPhysicalAtk(mPhysicalAttackPower - mValue); };
	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void RageClaw::Update()
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

void RageClaw::Render(HDC hdc)
{
	CAMERA->ScaleRender(hdc, mImage, mRect.left, mRect.top, mSizeX, mSizeY);

	if (mIsCollision)
	{
		SetBkMode(hdc, TRANSPARENT);
		CallFont(hdc, 15, [&]()
		{
			TextOut(hdc, mRect.left - CAMERA->GetRect().left, mRect.top - 40 - CAMERA->GetRect().top, mItemName.c_str(), mItemName.size());
			TextOut(hdc, mRect.left - CAMERA->GetRect().left, mRect.top - 25 - CAMERA->GetRect().top, mExplanation.c_str(), mExplanation.size());
			TextOut(hdc, mRect.left - CAMERA->GetRect().left, mRect.top - 10 - CAMERA->GetRect().top, mEffect.c_str(), mEffect.size());
		});
		SetBkMode(hdc, OPAQUE);

	}

}

void RageClaw::Release()
{

}
