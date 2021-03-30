#include "pch.h"
#include "AssassinationDagger.h"
#include "Inventory.h"
#include "Effect.h"

AssassinationDagger::AssassinationDagger(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "AssassinationDagger";

	mItemName = L"암살용 단검";
	mExplanation = L"봐서 알겠지만 베는 칼이 아니라 찌르는 칼이다";
	mEffect = L"공격속도가 25 %, 이동속도가 15 % 증가합니다.";

	IMAGEMANAGER->LoadFromFile(L"Madness", Resources(L"/item/Madness.bmp"), 78, 78, true);
	mSlot1Name = L"광기잇";
	mSlot1Image = IMAGEMANAGER->FindImage(L"Madness");
	mSlot1Explanation = L"적에게 기본공격으로 데미지를 입힐 시 5초간 공격속도가 5%씩 최대 25/45/75/125%까지 증가합니다.";

	IMAGEMANAGER->LoadFromFile(L"AssassinationDagger", Resources(L"item/AssassinationDagger1.bmp"), 23, 25, true);
	mImage = IMAGEMANAGER->FindImage(L"AssassinationDagger");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mType = ItemType::CommonItem;

	mValue = mAttackSpeed / 4.f;
	mSpeedValue = mInitSpeed*0.15f;

	mActivationFunc = []() {};
	mDeactivationFunc = [this]() 
	{
		SKUL->SetAtkSpeed(mAttackSpeed + mValue); 
		SKUL->GetCurrentSkul()->SetAttackSpeed();
		if(SKUL->GetAlterSkul())SKUL->GetAlterSkul()->SetAttackSpeed();
		SKUL->SetInitMovingSpeed(mInitSpeed - mSpeedValue);
	};
	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void AssassinationDagger::Update()
{
	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetRect();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //획득 트리거
		{
			SKUL->GetInventory()->GetItem(this);
			SKUL->SetAtkSpeed(mAttackSpeed - mValue);
			SKUL->GetCurrentSkul()->SetAttackSpeed();
			if (SKUL->GetAlterSkul())SKUL->GetAlterSkul()->SetAttackSpeed();
			SKUL->SetInitMovingSpeed(mInitSpeed + mSpeedValue);
			mIsTrashed = false;
			SetObjectOnTile(0, 0); //안보이는 어디론가로 숨겨놓는다... 이러면 어차피 클리핑되서 렌더도 안 돈다;
			mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
		}

		if (INPUT->GetKey('F') and mDuration >= 0)
		{
			mDuration -= dTime;

			SetObjectOnTile(mIndexX, mIndexY);
			mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
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
		new Effect(L"Spark", mX, mY, EffectType::Normal);
		SetObjectOnTile(1, 1); // 적당히 어딘가 쳐박아 두기
		mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	}
}

void AssassinationDagger::Render(HDC hdc)
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

void AssassinationDagger::Release()
{

}
