#include "pch.h"
#include "GoldenIncenseBurner.h"
#include "Inventory.h"
#include "Effect.h"

GoldenIncenseBurner::GoldenIncenseBurner(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "GoldenIncenseBurner";

	mItemName = L"황금향로";
	mExplanation = L"이걸 파는 상인들은 황금알을 낳는 거위의 배를 가른 것이다!";
	mEffect = L"획득하는 골드량이 20% 증가합니다.";

	IMAGEMANAGER->LoadFromFile(L"Scruge", Resources(L"/item/Scruge.bmp"), 78, 78, true);
	mSlot1Name = L"수전노";
	mSlot1Image = IMAGEMANAGER->FindImage(L"Scruge");
	mSlot1Explanation = L"획득하는 골드량이 3/12/25% 증가합니다.";

	IMAGEMANAGER->LoadFromFile(L"GoldenIncenseBurner", Resources(L"item/GoldenIncenseBurner1.bmp"), 20, 26, true);
	mImage = IMAGEMANAGER->FindImage(L"GoldenIncenseBurner");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mType = ItemType::CommonItem;

	mValue =20.f;
	mActivationFunc = []() {};
	mDeactivationFunc = []() {SKUL->SetGoldBonusRatio(SKUL->GetGoldBonusRatio() - 20.f);};
	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void GoldenIncenseBurner::Update()
{
	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetRect();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //획득 트리거
		{
			SKUL->GetInventory()->GetItem(this);
			SKUL->SetGoldBonusRatio(SKUL->GetGoldBonusRatio() + 20.f);
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

void GoldenIncenseBurner::Render(HDC hdc)
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

void GoldenIncenseBurner::Release()
{

}
