#include "pch.h"
#include "HunterHead.h"
#include "Hunter.h"
#include "Inventory.h"
#include "Effect.h"

HunterHead::HunterHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "HunterHead";

	mItemName = L"헌터";
	mExplanation = L"활과 화살로 영웅이 되고자 했던 모험가.";
	mEffect = L"공격 버튼을 누르고 있으면 정신을 집중하며 정신집중 완료 시 공격이 강화됩니다.\n\n백스텝 샷\n교대 시, 뒤로 이동하여 적을 관통하는 화살을 발사해 물리데미지를 입힙니다.";

	mSlot1Name = L"멀티플 샷";
	mSlot1Explanation = L"전방으로 다수의 화살을 발사해\n물리데미지를 입힙니다. \n\n쿨다운 8초.";

	mSlot2Name = L"피어스 샷";
	mSlot2Explanation = L"전방의 적을 꿰뚫는 마법화살을\n발사해 마법데미지를 입힙니다.\n\n쿨다운 14초.";

	mType = ItemType::SkulHead;

	IMAGEMANAGER->LoadFromFile(L"HunterHead", Resources(L"skulImage/HunterHead.bmp"), 27, 22, true);
	

	mImage = IMAGEMANAGER->FindImage(L"HunterHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"HunterSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"HunterSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void HunterHead::Update()
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
			Player* input = new Hunter(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
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

void HunterHead::Render(HDC hdc)
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

void HunterHead::Release()
{
}
