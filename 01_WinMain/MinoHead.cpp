#include "pch.h"
#include "MinoHead.h"
#include "Mino.h"
#include "Inventory.h"
#include "Effect.h"

MinoHead::MinoHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "MinoHead";

	mItemName = L"미노타우로스";
	mExplanation = L"크레타 미궁을 지키던 마물.";
	mEffect = L"스킬 사용 시 3초간 주변 적에게 물리데미지를 입힙니다.\n태클: 대쉬 시 공격판정, 연속대쉬 불가\n\n카우 어퍼컷\n교대 시, 강력한 어퍼컷으로 물리데미지를 입힙니다.";

	mSlot1Name = L"박치기";
	mSlot1Explanation = L"돌진하여 부딪힌 적을 날려버리고\n물리데미지를 입힙니다. \n\n쿨다운 10초.";

	mSlot2Name = L"둔기강타";
	mSlot2Explanation = L"돌진하여 물리데미지를 입힙니다.\n돌진공격이 종료되기 전에 버튼을 한 번 더 누를 시 둔기로 후려쳐 물리데미지를 입히고 \n기절을 부여합니다.\n\n쿨다운 14초.";


	IMAGEMANAGER->LoadFromFile(L"MinoHead", Resources(L"skulImage/MinotaurusHead.bmp"), 38, 35, true);
	IMAGEMANAGER->LoadFromFile(L"MinoSkill1", Resources(L"skillImage/MinoSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"MinoSkill2", Resources(L"skillImage/MinoSkill2.bmp"), 24, 24, true);

	mImage = IMAGEMANAGER->FindImage(L"MinoHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"MinoSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"MinoSkill2");
	mType = ItemType::SkulHead;

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	mType = ItemType::SkulHead;

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void MinoHead::Update()
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
			Player* input = new Mino(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
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

void MinoHead::Render(HDC hdc)
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

void MinoHead::Release()
{
}
