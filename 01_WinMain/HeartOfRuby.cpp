#include "pch.h"
#include "HeartOfRuby.h"
#include "Inventory.h"
#include "Effect.h"

HeartOfRuby::HeartOfRuby(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "HeartOfRuby";

	mItemName = L"루비심장";
	mExplanation = L"칼레온 마도집회에서 자랑 하듯 내세웠던 뱀파이어 혈액 연구의 결과물";
	mEffect = L"최대체력이 200이상일시 고정 피해 감소 수치(2)가 적용됩니다.";

	IMAGEMANAGER->LoadFromFile(L"Heart", Resources(L"/item/Heart.bmp"), 78, 78, true);
	mSlot1Name = L"심장";
	mSlot1Image = IMAGEMANAGER->FindImage(L"Heart");
	mSlot1Explanation = L"최대 체력이 20/35/60/135 증가합니다.";

	IMAGEMANAGER->LoadFromFile(L"HeartOfRuby", Resources(L"item/HeartOfRuby1.bmp"), 20, 21, true);
	mImage = IMAGEMANAGER->FindImage(L"HeartOfRuby");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mType = ItemType::CommonItem;
	mIsApplied = false;

	mActivationFunc = [this]() 
	{
		if (SKUL->GetMaxHp() >= 200 and mIsApplied==false)
		{
			mIsApplied = true;
			SKUL->SetDamageDecrease(SKUL->GetDamageDecrease() + 2);
		}
	};
	mDeactivationFunc = [this]()
	{
		if (mIsApplied)
		{
			SKUL->SetDamageDecrease(SKUL->GetDamageDecrease() - 2);
		}
	};
	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void HeartOfRuby::Update()
{
	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetRect();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //획득 트리거
		{
			SKUL->GetInventory()->GetItem(this);
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

void HeartOfRuby::Render(HDC hdc)
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

void HeartOfRuby::Release()
{

}
