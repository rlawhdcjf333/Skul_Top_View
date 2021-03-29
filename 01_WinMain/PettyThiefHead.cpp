#include "pch.h"
#include "PettyThiefHead.h"
#include "PettyThief.h"
#include "Inventory.h"
#include "Effect.h"

PettyThiefHead::PettyThiefHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "PettyThiefHead";

	mItemName = L"좀도둑";
	mExplanation = L"마족 생명보험을 들었더니 돈을 주는게 아니라 해골로 부활시켜주지 뭐야!";
	mEffect = L"적에게 기본 공격으로 데미지를 입힐 시 10% 확률로 3 금화가 떨어집니다.\n구르기: 대쉬거리 증가.\n\n길거리 생존법\n교대 시, 뒤로 이동하여 참격으로 물리데미지를 입힙니다.";

	mSlot1Name = L"주머니 찢기";
	mSlot1Explanation = L"무적상태로 단검을 휘둘러 물리데미지를 입힙니다.\n피격된 적으로부터 2 금화가 떨어집니다.\n\n쿨다운 8초.";

	mSlot2Name = L"뒤구르기";
	mSlot2Explanation = L"작은 연막을 일으켜 마법데미지를 입히고\n무적상태로 뒤로 굴러 적의 공격을 회피합니다.\n이 스킬은 최대 3회 충전할 수 있습니다..\n\n쿨다운 7초.";


	IMAGEMANAGER->LoadFromFile(L"PettyThiefHead", Resources(L"skulImage/PettyThiefHead.bmp"), 21, 21, true);
	IMAGEMANAGER->LoadFromFile(L"PettyThiefSkill1", Resources(L"skillImage/PettyThiefSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"PettyThiefSkill2", Resources(L"skillImage/PettyThiefSkill2.bmp"), 24, 24, true);

	mImage = IMAGEMANAGER->FindImage(L"PettyThiefHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"PettyThiefSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"PettyThiefSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void PettyThiefHead::Update()
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
			Player* input = new PettyThief(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
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

void PettyThiefHead::Render(HDC hdc)
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

void PettyThiefHead::Release()
{
}
