#include "pch.h"
#include "HeartOfNaias.h"
#include "Inventory.h"
#include "Effect.h"

HeartOfNaias::HeartOfNaias(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "HeartOfNaias";

	mItemName = L"나이아스의 심장";
	mExplanation = L"정령 호수를 지키던 요정의 가슴 속에 응결 된 마법의 보석";
	mEffect = L"최대체력이 60 증가합니다.";

	IMAGEMANAGER->LoadFromFile(L"Heart", Resources(L"/item/Heart.bmp"), 78, 78, true);
	mSlot1Name = L"심장";
	mSlot1Image = IMAGEMANAGER->FindImage(L"Heart");
	mSlot1Explanation = L"최대 체력이 20/35/60/135 증가합니다.";

	IMAGEMANAGER->LoadFromFile(L"HeartOfNaias", Resources(L"item/HeartOfNaias1.bmp"), 24, 22, true);
	mImage = IMAGEMANAGER->FindImage(L"HeartOfNaias");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mType = ItemType::CommonItem;

	mValue = 60;

	mActivationFunc = []() {};
	mDeactivationFunc = [this]() {SKUL->SetMaxHp(SKUL->GetMaxHp() - mValue); if (SKUL->GetHp() > SKUL->GetMaxHp()) SKUL->SetHp(SKUL->GetMaxHp());};
	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void HeartOfNaias::Update()
{
	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetRect();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //획득 트리거
		{
			SKUL->GetInventory()->GetItem(this);
			SKUL->SetMaxHp(SKUL->GetMaxHp() + mValue);
			SKUL->PlusHp(mValue);
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

void HeartOfNaias::Render(HDC hdc)
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

void HeartOfNaias::Release()
{

}
