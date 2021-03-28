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

	mItemName =L"������ Į���� ���";
	mExplanation = L"������ Į���� �� ���޿� ��. ������ �ǽ��� ������, ������ �˳��� ������� �ʴ�.";
	mEffect = L"�������ݷ��� 25% �����մϴ�.";
	
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

		if (INPUT->GetKeyUp('F') and mDuration>=0) //ȹ�� Ʈ����
		{
			SKUL->GetInventory()->GetItem(this);
			SKUL->SetPhysicalAtk(mPhysicalAttackPower + mValue);
			mIsTrashed = false;
			SetObjectOnTile(0, 0); //�Ⱥ��̴� ���а��� ���ܳ��´�... �̷��� ������ Ŭ���εǼ� ������ �� ����;
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

	if (mDuration < 0) //������ �ı� Ʈ����
	{
		SetObjectOnTile(1, 1); // ������ ��� �Ĺھ� �α�
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
