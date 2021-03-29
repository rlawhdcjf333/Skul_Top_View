#include "pch.h"
#include "EntHead.h"
#include "Inventory.h"
#include "Effect.h"
#include "Ent.h"


EntHead::EntHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "EntHead";

	mItemName = L"엔트 스컬";
	mExplanation = L"조화의 숲의 일원이었던 작은 엔트.";
	mEffect = L"기본공격 시 30% 확률로 추가공격이 발생하여 물리데미지를 입힙니다.\n태클: 대쉬 시 공격판정, 연속대쉬 불가.\n\n친환경 등장\n교대 시 무적상태로 지면에 숨어있다가 솟아나며 물리데미지를 입힙니다.";

	mSlot1Name = L"나무 돌진";
	mSlot1Explanation = L"지면을 연속으로 내려찍으며 나아가\n물리데미지를 입힙니다.\n\n쿨다운 11초.";

	mSlot2Name = L"급성장";
	mSlot2Explanation = L"거대한 주먹으로 내려찍어 물리데미지를 입힙니다.\n버튼을 누르고 있으면 정신을 집중하며 정신집중\n완료 시 공격범위와 데미지가 증가합니다.\n\n쿨다운 13초.";


	IMAGEMANAGER->LoadFromFile(L"EntHead", Resources(L"skulImage/EntHead.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"EntSkill1", Resources(L"skillImage/EntSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"EntSkill2", Resources(L"skillImage/EntSkill2.bmp"), 24, 24, true);

	mImage = IMAGEMANAGER->FindImage(L"EntHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"EntSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"EntSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void EntHead::Update()
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
			Player* input = new Ent(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
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

void EntHead::Render(HDC hdc)
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

void EntHead::Release()
{
}
