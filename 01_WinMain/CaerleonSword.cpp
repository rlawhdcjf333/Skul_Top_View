#include "pch.h"
#include "CaerleonSword.h"
#include "Inventory.h"
#include "Effect.h"

CaerleonSword::CaerleonSword(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "CaerleonSword";

	mItemName =L"보급형 칼레온 장검";
	mExplanation = L"묵직한 칼레온 군 보급용 검. 성능은 의심이 들지만, 관리된 검날은 예사롭지 않다.";
	mEffect = L"물리공격력이 25% 증가합니다.";
	
	IMAGEMANAGER->LoadFromFile(L"CaerleonSword", Resources(L"item/CaerleonSword.bmp"), 22, 22, true);
	mImage = IMAGEMANAGER->FindImage(L"CaerleonSword");
	
	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mValue = mPhysicalAttackPower / 4;
	if (mValue == 0) mValue = 1;

	mDeactivationFunc = [this]() {SKUL->SetPhysicalAtk(mPhysicalAttackPower - mValue);};
	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void CaerleonSword::Update()
{
	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetRect();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration>=0) //획득 트리거
		{
			SKUL->GetInventory()->GetItem(this);
			SKUL->SetPhysicalAtk(mPhysicalAttackPower + mValue);
			mIsTrashed = false;
			SetObjectOnTile(0, 0); //안보이는 어디론가로 숨겨놓는다... 이러면 어차피 클리핑되서 렌더도 안 돈다;
			mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
		}

		if (INPUT->GetKey('F'))
		{
			mDuration -= dTime;
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
		SetObjectOnTile(1, 1); // 적당히 어딘가 쳐박아 두기
		mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
		new Effect(L"Spark", mX, mY, EffectType::Normal);
	}
}

void CaerleonSword::Render(HDC hdc)
{
	CAMERA->ScaleRender(hdc, mImage, mRect.left, mRect.top, mSizeX, mSizeY);

	if (mIsCollision)
	{
		TextOut(hdc, mRect.left-CAMERA->GetRect().left, mRect.top - 40 - CAMERA->GetRect().top, mItemName.c_str(), mItemName.size());
		TextOut(hdc, mRect.left - CAMERA->GetRect().left, mRect.top - 25 - CAMERA->GetRect().top, mExplanation.c_str(), mExplanation.size());
		TextOut(hdc, mRect.left - CAMERA->GetRect().left, mRect.top - 10 - CAMERA->GetRect().top, mEffect.c_str(), mEffect.size());
	}
	
}

void CaerleonSword::Release()
{

}
