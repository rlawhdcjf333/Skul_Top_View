#include "pch.h"
#include "AlchemistHead.h"
#include "Alchemist.h"
#include "Inventory.h"
#include "Effect.h"

AlchemistHead::AlchemistHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "AlchemistHead";

	mItemName = L"연금술사";
	mExplanation = L"대체 어디서부터 잘못된 걸까...";
	mEffect = L"10초마다 불완전한 검은마석 골렘을 소환합니다.(마법 데미지)\n\n연구성과\n교대 시, 사방에 플라스크를 흩뿌립니다.(마법 데미지)";

	mSlot1Name = L"역병 플라스크";
	mSlot1Explanation = L"닿으면 폭발하는 플라스크를 던집니다.\n(마법 데미지)\n이 스킬은 8회 충전할 수 있습니다.\n\n충전 4초.";

	mSlot2Name = L"화염 플라스크";
	mSlot2Explanation = L"닿으면 폭발하는 플라스크를 던집니다.\n(마법 데미지)\n적 공격 시 화상 부여(화상: 마법 데미지)\n이 스킬은 5회 충전할 수 있습니다.\n\n충전 13초.";

	mType = ItemType::SkulHead;

	IMAGEMANAGER->LoadFromFile(L"AlchemistHead", Resources(L"skulImage/AlchemistHead.bmp"), 26, 21, true);
		
	mImage = IMAGEMANAGER->FindImage(L"AlchemistHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"AlchemistSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"AlchemistSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void AlchemistHead::Update()
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
			Player* input = new Alchemist(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
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

void AlchemistHead::Render(HDC hdc)
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

void AlchemistHead::Release()
{
}
