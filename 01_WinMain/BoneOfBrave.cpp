#include "pch.h"
#include "BoneOfBrave.h"
#include "Inventory.h"
#include "Effect.h"

BoneOfBrave::BoneOfBrave(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "BoneOfBrave";

	mItemName = L"용기의 뼈";
	mExplanation = L"묵직한 것을 보니 이 뼈는 아마도 물에 뜨지 않을 것 같다.";
	mEffect = L"3초마다 다음 물리공격의 데미지가 20% 증폭됩니다.";

	IMAGEMANAGER->LoadFromFile(L"March", Resources(L"/item/March.bmp"), 78, 78, true);
	mSlot1Name = L"행군";
	mSlot1Image = IMAGEMANAGER->FindImage(L"March");
	mSlot1Explanation = L"물리공격력이 15/25/40/100% 증폭됩니다.";

	IMAGEMANAGER->LoadFromFile(L"BoneOfBrave", Resources(L"item/BoneOfBrave1.bmp"), 26, 26, true);
	mImage = IMAGEMANAGER->FindImage(L"BoneOfBrave");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mType = ItemType::CommonItem;
	
	mDelay = 3.f;
	mIsEffective = true;
	mValue = mPhysicalAttackPower / 5.f;

	mActivationFunc = [this]() 
	{
		mDelay -= dTime;
		if (mDelay < 0) mDelay = 0;

		if (mDelay == 0 and Obj->EnemyHitCheck())
		{
			mDelay = 3.f;
			Obj->DamageUpToEnemy(mValue); //20프로 추뎀 적용
		}
	};
	mDeactivationFunc = []() {};
	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void BoneOfBrave::Update()
{
	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetRect();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //획득 트리거
		{
			SKUL->GetInventory()->GetItem(this);
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

void BoneOfBrave::Render(HDC hdc)
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

void BoneOfBrave::Release()
{

}
