#include "pch.h"
#include "InfinityBone.h"
#include "Inventory.h"
#include "Effect.h"

InfinityBone::InfinityBone(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "InfinityBone";

	mItemName = L"무한의 뼈";
	mExplanation = L"여기라면 마녀님의 두루마리를 정말 안전하게 보호할 수 있다.";
	mEffect = L"비충전형 스킬 사용 시 30% 확률로 해당 스킬의 쿨타임이 초기화 됩니다.";

	IMAGEMANAGER->LoadFromFile(L"BlackMagic", Resources(L"/item/BlackMagic.bmp"), 78, 78, true);
	mSlot1Name = L"흑마술";
	mSlot1Image = IMAGEMANAGER->FindImage(L"BlackMagic");
	mSlot1Explanation = L"스킬 쿨다운 속도가 20/40/70/120% 증가합니다.";

	IMAGEMANAGER->LoadFromFile(L"InfinityBone", Resources(L"item/InfinityBone1.bmp"), 26, 22, true);
	mImage = IMAGEMANAGER->FindImage(L"InfinityBone");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mType = ItemType::CommonItem;

	mValue = 0;

	mSkillCheck1 = false;
	mSkillCheck2 = false;

	mActivationFunc = [this]()
	{
		if (SKUL->GetCurrentSkul()->GetSkill1CoolTime() == 0)
		{
			mSkillCheck1 = false;
		}
		if (mSkillCheck1 == false and SKUL->GetCurrentSkul()->GetSkill1CoolTime() > 0)
		{
			mSkillCheck1 = true;
			if (RAND->Probablity(30)) SKUL->GetCurrentSkul()->SetSkill1CoolTime(0);
		}
		if (SKUL->GetCurrentSkul()->GetSkill2CoolTime() == 0)
		{
			mSkillCheck2 = false;
		}
		if (mSkillCheck2 == false and SKUL->GetCurrentSkul()->GetSkill2CoolTime() > 0)
		{
			mSkillCheck2 = true;
			if (RAND->Probablity(30)) SKUL->GetCurrentSkul()->SetSkill2CoolTime(0);
		}
	};
	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void InfinityBone::Update()
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

void InfinityBone::Render(HDC hdc)
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

void InfinityBone::Release()
{

}
