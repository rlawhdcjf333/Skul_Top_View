#include "pch.h"
#include "BoneOfMana.h"
#include "Inventory.h"
#include "Effect.h"

BoneOfMana::BoneOfMana(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "BoneOfMana";

	mItemName = L"마나의 뼈";
	mExplanation = L"대체 어떤 부유한 스켈레톤이 이런걸 만들어서 쓸까?";
	mEffect = L"스킬이 시전 준비되거나 충전될 때마다 자신 주변에 마나 회오리를 일으킵니다.";

	IMAGEMANAGER->LoadFromFile(L"BoneOfManaEffect", Resources(L"item/BoneOfManaEffect.bmp"), 500, 500, 5, 5, true);

	IMAGEMANAGER->LoadFromFile(L"Strategy", Resources(L"/item/Strategy.bmp"), 78, 78, true);
	mSlot1Name = L"전술";
	mSlot1Image = IMAGEMANAGER->FindImage(L"Strategy");
	mSlot1Explanation = L"마법공격력이 15/25/40/100% 증폭됩니다.";

	IMAGEMANAGER->LoadFromFile(L"BoneOfMana", Resources(L"item/BoneOfMana1.bmp"), 26, 26, true);
	mImage = IMAGEMANAGER->FindImage(L"BoneOfMana");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mType = ItemType::CommonItem;

	mValue = 0;
	mActivationFunc = []() 
	{
		if ((SKUL->GetCurrentSkul()->GetSkill1CoolTime() > 0 and SKUL->GetCurrentSkul()->GetSkill1CoolTime() <= dTime)
			or (SKUL->GetCurrentSkul()->GetSkill2CoolTime() > 0 and SKUL->GetCurrentSkul()->GetSkill2CoolTime() <= dTime))
		{
			SKUL->GetCurrentSkul()->Attack(mMagicalAttackPower, 2, AttackType::Whirlwind);
			(new Effect(L"BoneOfManaEffect", SKUL->GetCurrentSkul()->GetX(), SKUL->GetCurrentSkul()->GetY(), EffectType::Normal))->Scaling(200,200,0.5f);
		}
	};

	mDeactivationFunc = []() {};
	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void BoneOfMana::Update()
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

void BoneOfMana::Render(HDC hdc)
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

void BoneOfMana::Release()
{

}
