#include "pch.h"
#include "MagesNecklace.h"
#include "Inventory.h"
#include "Effect.h"

MagesNecklace::MagesNecklace(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "MagesNecklace";

	mItemName = L"마법사의 목걸이";
	mExplanation = L"목걸이에 박힌 보석의 영향으로 마나의 흐름을 더욱 잘 느낄 수 있게된다.";
	mEffect = L"스킬을 사용할 때마다 교대 쿨타임이 0.5초 감소합니다.스킬 사용 시 화염옥이 생성됩니다.\n화염옥은 일정범위 내 가장 가까운 적에게 파이어볼을 발사하여 마법데미지를 입힙니다.";

	IMAGEMANAGER->LoadFromFile(L"BlackMagic", Resources(L"/item/BlackMagic.bmp"), 78, 78, true);
	mSlot1Name = L"흑마술";
	mSlot1Image = IMAGEMANAGER->FindImage(L"BlackMagic");
	mSlot1Explanation = L"스킬 쿨다운 속도가 20/40/70/120% 증가합니다.";

	IMAGEMANAGER->LoadFromFile(L"MagesNecklace", Resources(L"item/MagesNecklace1.bmp"), 24, 22, true);
	mImage = IMAGEMANAGER->FindImage(L"MagesNecklace");

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

void MagesNecklace::Update()
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

void MagesNecklace::Render(HDC hdc)
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

void MagesNecklace::Release()
{

}
