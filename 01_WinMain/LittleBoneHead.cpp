#include "pch.h"
#include "LittleBoneHead.h"
#include "Inventory.h"
#include "Effect.h"


LittleBoneHead::LittleBoneHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "LittleBoneHead";

	mItemName = L"리틀본";
	mExplanation = L"마왕성 경비대의 막내 스켈레톤. 다른 스켈레톤에 비해 체구가 작아 리틀본이라 불린다.";
	mEffect = L"\n교대 시 뼈를 들고 회전해 물리 데미지를 입힙니다.";

	mSlot1Name = L"두개골 투척";
	mSlot1Explanation = L"자신의 머리를 던져 마법 데미지를 입힙니다. \n쿨다운 4초.";
	
	mSlot2Name = L"머리가 본체";
	mSlot2Explanation =L"머리가 없는 상태일 때, 머리로 이동합니다.";


	IMAGEMANAGER->LoadFromFile(L"LittleBoneHead", Resources(L"item/LittleBoneHead.bmp"), 22, 22, true);
	mImage = IMAGEMANAGER->FindImage(L"LittleBoneHead");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void LittleBoneHead::Update()
{
	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetRect();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration >= 0) //획득 트리거
		{
			SKUL->GetInventory()->GetSkul(this);
			mIsTrashed = false;
			SetObjectOnTile(0, 0); //안보이는 어디론가로 숨겨놓는다... 이러면 어차피 클리핑되서 렌더도 안 돈다;
			mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
		}

		if (INPUT->GetKey('F'))
		{
			mDuration -= dTime;
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
		SetObjectOnTile(1, 1); // 적당히 어딘가 쳐박아 두기
		mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
		new Effect(L"Spark", mX, mY, EffectType::Normal);
	}
}

void LittleBoneHead::Render(HDC hdc)
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

void LittleBoneHead::Release()
{
}
