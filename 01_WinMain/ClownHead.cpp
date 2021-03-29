#include "pch.h"
#include "ClownHead.h"
#include "Clown.h"
#include "Inventory.h"
#include "Effect.h"

ClownHead::ClownHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "ClownHead";

	mItemName = L"광대";
	mExplanation = L"이 사과를 네 두개골에 놔. - 단검 곡예사 2/1";
	mEffect = L"나이프로 데미지를 입힐 시 표식을 남기며 표식이 3개가 되면 폭발해 마법 데미지를 입힙니다.\n구르기: 대쉬거리 증가, 연속대쉬 불가\n\n쇼타임\n교대 시, 백덤블링하여 다수의 나이프를 던져 물리 데미지를 입힙니다.";

	mSlot1Name = L"깜짝 나이프";
	mSlot1Explanation = L"닿으면 폭발하는 나이프를 3개 던져 \n마법 데미지를 입힙니다.\n\n쿨다운 12초.";

	mSlot2Name = L"깜짝 상자";
	mSlot2Explanation = L"빠르게 달려나가며 깜짝상자를 뿌립니다.\n깜짝 상자는 지면에 닿아 폭발하여\n물리 데미지를 입힙니다.\n\n쿨다운 14초.";


	IMAGEMANAGER->LoadFromFile(L"ClownHead", Resources(L"skulImage/ClownHead.bmp"), 37, 26, true);
	IMAGEMANAGER->LoadFromFile(L"ClownSkill1", Resources(L"skillImage/ClownSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"ClownSkill2", Resources(L"skillImage/ClownSkill2.bmp"), 24, 24, true);

	mImage = IMAGEMANAGER->FindImage(L"ClownHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"ClownSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"ClownSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void ClownHead::Update()
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
			Player* input = new Clown(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
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

void ClownHead::Render(HDC hdc)
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

void ClownHead::Release()
{
}
