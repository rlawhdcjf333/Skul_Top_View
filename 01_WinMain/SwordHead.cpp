#include "pch.h"
#include "SwordHead.h"
#include "Sword.h"
#include "Inventory.h"
#include "Effect.h"

SwordHead::SwordHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "SwordHead";

	mItemName = L"스켈레톤 검";
	mExplanation = L"이름없는 스켈레톤 병사 중 하나.";
	mEffect = L"적 공격 시 5% 확률로 출혈을 가합니다.\n\n거친 칼날\n교대 시, 6초 간 모든 공격에 출혈을 가합니다.";

	mSlot1Name = L"녹슨 찌르기";
	mSlot1Explanation = L"돌진하여 물리 데미지를 입힙니다.\n\n쿨다운 11초.";

	mSlot2Name = L"트리플 피어스";
	mSlot2Explanation = L"검으로 3번 찔러 물리 데미지를 입힙니다.\n\n쿨다운 6초.";


	IMAGEMANAGER->LoadFromFile(L"SwordHead", Resources(L"skulImage/SwordHead.bmp"), 23, 20, true);
	IMAGEMANAGER->LoadFromFile(L"SwordSkill1", Resources(L"skillImage/SwordSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"SwordSkill2", Resources(L"skillImage/SwordSkill2.bmp"), 24, 24, true);

	mImage = IMAGEMANAGER->FindImage(L"SwordHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"SwordSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"SwordSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void SwordHead::Update()
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
			Player* input = new Sword(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
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

void SwordHead::Render(HDC hdc)
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

void SwordHead::Release()
{
}
