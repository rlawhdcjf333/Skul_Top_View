#include "pch.h"
#include "BerserkersGlove.h"
#include "Inventory.h"
#include "Effect.h"

BerserkersGlove::BerserkersGlove(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "BerserkersGlove";

	mItemName = L"�������� �尩";
	mExplanation = L"�� �� �� ���� ���� �̰� �ȾҴٰ�, �� ���� ��ü�� ������ع��� ���� �־���.- ������ ����";
	mEffect = L"ü���� ���� �����̸� ���ݼӵ��� 30% ����մϴ�.";

	IMAGEMANAGER->LoadFromFile(L"Madness", Resources(L"/item/Madness.bmp"), 78, 78, true);
	mSlot1Name = L"������";
	mSlot1Image = IMAGEMANAGER->FindImage(L"Madness");
	mSlot1Explanation = L"������ �⺻�������� �������� ���� �� 5�ʰ� ���ݼӵ��� 5%�� �ִ� 25/45/75/125%���� �����մϴ�.";

	IMAGEMANAGER->LoadFromFile(L"BerserkersGlove", Resources(L"item/BerserkersGlove1.bmp"), 19, 23, true);
	mImage = IMAGEMANAGER->FindImage(L"BerserkersGlove");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mType = ItemType::CommonItem;

	mIsApplied=false;
	mValue = 0.03;

	mActivationFunc = [this]()
	{
		if (SKUL->GetLostHpPercentage() >= 50 and mIsApplied==false)
		{
			mIsApplied = true;
			SKUL->SetPhysicalAtk(mAttackSpeed - mValue);
			SKUL->GetCurrentSkul()->SetAttackSpeed();
			if (SKUL->GetAlterSkul()) SKUL->GetAlterSkul()->SetAttackSpeed();
		}
		if (SKUL->GetLostHpPercentage() < 50 and mIsApplied==true)
		{
			mIsApplied = false;
			SKUL->SetPhysicalAtk(mAttackSpeed + mValue);
			SKUL->GetCurrentSkul()->SetAttackSpeed();
			if (SKUL->GetAlterSkul()) SKUL->GetAlterSkul()->SetAttackSpeed();
		}


	};
	mDeactivationFunc = [this]()
	{
		if (mIsApplied == true)
		{
			SKUL->SetPhysicalAtk(mAttackSpeed + mValue);
			SKUL->GetCurrentSkul()->SetAttackSpeed();
			if (SKUL->GetAlterSkul()) SKUL->GetAlterSkul()->SetAttackSpeed();
		}
	};
	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void BerserkersGlove::Update()
{
	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetRect();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //ȹ�� Ʈ����
		{
			SKUL->GetInventory()->GetItem(this);
			mIsTrashed = false;
			SetObjectOnTile(0, 0); //�Ⱥ��̴� ���а��� ���ܳ��´�... �̷��� ������ Ŭ���εǼ� ������ �� ����;
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

	if (mDuration < 0) //������ �ı� Ʈ����
	{
		new Effect(L"Spark", mX, mY, EffectType::Normal);
		SetObjectOnTile(1, 1); // ������ ��� �Ĺھ� �α�
		mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	}
}

void BerserkersGlove::Render(HDC hdc)
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

void BerserkersGlove::Release()
{

}
