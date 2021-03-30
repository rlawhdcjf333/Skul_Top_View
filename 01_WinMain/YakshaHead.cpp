#include "pch.h"
#include "YakshaHead.h"
#include "Yaksha.h"
#include "Inventory.h"
#include "Effect.h"

YakshaHead::YakshaHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "YakshaHead";

	mItemName = L"야차";
	mExplanation = L"마물을 짓밟기를 일삼던 거대한 도깨비.";
	mEffect = L"5번째로 발을 구를 시 전방에 큰 피해를 입힙니다.(물리 데미지)\n태클: 대쉬 시 공격판정, 연속대쉬 불가";

	mSlot1Name = L"야차정권";
	mSlot1Explanation = L"스킬버튼을 누르고 있으면 정신집중을 합니다.\n정신집중 완료시 스킬의 위력이 증가합니다.(물리 데미지)\n\n쿨다운 12초.";

	mSlot2Name = L"야차행진";
	mSlot2Explanation = L"앞으로 나아가며 발을 3번 구릅니다.(물리 데미지)\n\n쿨다운 8초.";


	IMAGEMANAGER->LoadFromFile(L"YakshaHead", Resources(L"skulImage/YakshaHead.bmp"), 25, 25, true);
	

	mImage = IMAGEMANAGER->FindImage(L"YakshaHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"YakshaSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"YakshaSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;
	mType = ItemType::SkulHead;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void YakshaHead::Update()
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
			Player* input = new Yaksha(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
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

void YakshaHead::Render(HDC hdc)
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

void YakshaHead::Release()
{
}
