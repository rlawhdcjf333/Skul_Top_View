#include "pch.h"
#include "BerserkerHead.h"
#include "Inventory.h"
#include "Effect.h"
#include "Berserker.h"


BerserkerHead::BerserkerHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "BerserkerHead";

	mItemName = L"버서커";
	mExplanation = L"타고난 광기로 대륙을 뒤흔들었던 흉포한 전사.";
	mEffect = L"이 스컬은 스킬 '뼈의 울음'을 반드시 갖습니다.\n잃은 체력에 비례하여 물리공격력이 최대 80 % 까지 증폭됩니다.\n피격 시 3초간 공격속도가 50 % 증가합니다.\n태클: 대쉬 시 공격판정, 연속대쉬 불가.\n교대 시 전방으로 돌진해 내려찍으며 물리 데미지를 입히고 5초간 물리 공격력이 50% 증가합니다.";

	mSlot1Name = L"뼈의 울음";
	mSlot1Explanation = L"포효하여 주변에 물리 데미지를 입히며 밀쳐내고,\n10초 동안 공격속도 + 100 % 가 됩니다.\n\n쿨다운 30초.";

	mSlot2Name = L"매드러시";
	mSlot2Explanation = L"전방으로 나아가며 지면을 수차례 내려찍은 뒤 크게 내려 찍어 강력한 물리 데미지를 입힙니다.\n\n쿨다운 12초.";

	mType = ItemType::SkulHead;

	IMAGEMANAGER->LoadFromFile(L"BerserkerHead", Resources(L"skulImage/BerserkerHead.bmp"), 35, 19, true);
	

	mImage = IMAGEMANAGER->FindImage(L"BerserkerHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"BerserkerSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"BerserkerSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void BerserkerHead::Update()
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
			Player* input = new Berserker(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
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

void BerserkerHead::Render(HDC hdc)
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

void BerserkerHead::Release()
{
}

