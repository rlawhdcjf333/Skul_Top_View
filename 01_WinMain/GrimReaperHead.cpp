#include "pch.h"
#include "GrimReaperHead.h"
#include "GrimReaper.h"
#include "Inventory.h"
#include "Effect.h"

GrimReaperHead::GrimReaperHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "GrimReaperHead";

	mItemName = L"그림리퍼";
	mExplanation = L"죽음을 관장하는 명계의 신.";
	mEffect = L"적 처치 시 영혼불이 나타납니다.\n영혼불은 주변의 다른 적을 추적하여 폭발해 마법 데미지를 입힙니다.\n\n죽음의 인도\n교대 시 무적상태로 나타나 낫을 휘둘러 마법 데미지를 입힙니다.";

	mSlot1Name = L"선고";
	mSlot1Explanation = L"돌진하여 지난자리에 있던 적을 낫으로 베어 마법 데미지를 입힙니다.\n피격된 적에게 균열이 발생하여 마법 데미지를 입힙니다.\n\n쿨다운 12초.";

	mSlot2Name = L"수확";
	mSlot2Explanation = L"무적상태로 주변 적들의 멈춘 뒤\n낫을 크게 휘둘러 마법 데미지를 입힙니다.\n\n쿨다운 45초.";


	IMAGEMANAGER->LoadFromFile(L"GrimReaperHead", Resources(L"skulImage/GrimReaperHead.bmp"), 36, 24, true);
	IMAGEMANAGER->LoadFromFile(L"GrimReaperSkill1", Resources(L"skillImage/GrimReaperSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"GrimReaperSkill2", Resources(L"skillImage/GrimReaperSkill2.bmp"), 24, 24, true);

	mImage = IMAGEMANAGER->FindImage(L"GrimReaperHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"GrimReaperSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"GrimReaperSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void GrimReaperHead::Update()
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
			Player* input = new GrimReaper(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
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

void GrimReaperHead::Render(HDC hdc)
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

void GrimReaperHead::Release()
{
}
