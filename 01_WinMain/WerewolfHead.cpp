#include "pch.h"
#include "WerewolfHead.h"
#include "Werewolf.h"
#include "Inventory.h"
#include "Effect.h"

WerewolfHead::WerewolfHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "WerewolfHead";

	mItemName = L"웨어울프";
	mExplanation = L"빠른 발과 날카로운 손톱을 가졌었던 늑대 마족.";
	mEffect = L"이동속도와 대쉬거리가 10% 증가합니다.\n\n야생의 질주\n교대 시, 무적상태로 돌진하여 물리데미지를 입힙니다.";

	mSlot1Name = L"찢기";
	mSlot1Explanation = L"양손으로 적을 찢어 마법데미지를 입힙니다.\n\n쿨다운 5초.";

	mSlot2Name = L"포식";
	mSlot2Explanation = L"할퀴기 공격으로 마법데미지를 입힙니다.\n할퀴기 공격으로 처치한 적 1명당 체력을 1 회복합니다.\n\n쿨다운 15초.";


	IMAGEMANAGER->LoadFromFile(L"WerewolfHead", Resources(L"skulImage/WerewolfHead.bmp"), 29, 25, true);
	IMAGEMANAGER->LoadFromFile(L"WerewolfSkill1", Resources(L"skillImage/WereWolfSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"WerewolfSkill2", Resources(L"skillImage/WereWolfSkill2.bmp"), 24, 24, true);

	mImage = IMAGEMANAGER->FindImage(L"WerewolfHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"WerewolfSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"WerewolfSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void WerewolfHead::Update()
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
			Player* input = new Werewolf(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
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

void WerewolfHead::Render(HDC hdc)
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

void WerewolfHead::Release()
{
}
