#include "pch.h"
#include "LittleBoneHead.h"
#include "Inventory.h"
#include "Effect.h"
#include "LittleBone.h"


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
	mSlot1Explanation = L"자신의 머리를 던져 마법 데미지를 입힙니다. \n\n쿨다운 4초.";
	
	mSlot2Name = L"머리가 본체";
	mSlot2Explanation =L"머리가 없는 상태일 때, 머리로 이동합니다.";

	mType = ItemType::SkulHead;

	IMAGEMANAGER->LoadFromFile(L"LittleBoneHead", Resources(L"skulImage/LittleBoneHead.bmp"), 15, 13, true);
	IMAGEMANAGER->LoadFromFile(L"LittleBoneSkill1", Resources(L"skillImage/LittleBone1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"LittleBoneSkill2", Resources(L"skillImage/LittleBone2.bmp"), 24, 24, true);
	mImage = IMAGEMANAGER->FindImage(L"LittleBoneHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"LittleBoneSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"LittleBoneSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void LittleBoneHead::Update()
{
	mDelay -= dTime;
	if (mDelay <= 0)mDelay = 0;
	if (mDelay > 0) return;

	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetHitBox();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //획득 트리거
		{
			Player* input = new LittleBone(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
			input->Init();
			Obj->AddObject(ObjectLayer::Player, input);//새 스컬을 오브젝트 매니저에 등록
			SKUL->GetInventory()->GetSkul(this); // 인벤토리에 이 스컬 머리를 등록
			SKUL->NewSkulGet(input);//새 스컬을 스컬매니저에 등록
			mIsTrashed = false;
			SetObjectOnTile(0, 0); //안보이는 어디론가로 숨겨놓는다... 이러면 어차피 클리핑되서 렌더도 안 돈다;
		}

		if (INPUT->GetKey('F'))
		{
			mDuration -= dTime;

			SetObjectOnTile(mIndexX, mIndexY);
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
		new Effect(L"Debris", mX, mY, EffectType::Normal);
		SetObjectOnTile(1, 1); // 적당히 어딘가 쳐박아 두기
	}
}

void LittleBoneHead::Render(HDC hdc)
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

void LittleBoneHead::Release()
{
}
