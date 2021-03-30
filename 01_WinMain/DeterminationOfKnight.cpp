#include "pch.h"
#include "DeterminationOfKnight.h"
#include "Inventory.h"
#include "Effect.h"

DeterminationOfKnight::DeterminationOfKnight(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "DeterminationOfKnight";

	mItemName = L"����� ����";
	mExplanation = L"����� �ռ����� ���и� �ø� ä�� �׾� �ִ� �̸����� ����� ����";
	mEffect = L"���� ü���� 30% ������ ��� �޴� �������� 30% �����մϴ�.";

	IMAGEMANAGER->LoadFromFile(L"Heart", Resources(L"/item/Heart.bmp"), 78, 78, true);
	mSlot1Name = L"����";
	mSlot1Image = IMAGEMANAGER->FindImage(L"Heart");
	mSlot1Explanation = L"�ִ� ü���� 20/35/60/135 �����մϴ�.";

	IMAGEMANAGER->LoadFromFile(L"DeterminationOfKnight", Resources(L"item/DeterminationOfKnight1.bmp"), 28, 27, true);
	mImage = IMAGEMANAGER->FindImage(L"DeterminationOfKnight");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mType = ItemType::CommonItem;

	mValue = mPhysicalAttackPower / 4.f;

	mActivationFunc = []() {};
	mDeactivationFunc = [this]() {SKUL->SetPhysicalAtk(mPhysicalAttackPower - mValue); };
	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void DeterminationOfKnight::Update()
{
	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetRect();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //ȹ�� Ʈ����
		{
			SKUL->GetInventory()->GetItem(this);
			SKUL->SetPhysicalAtk(mPhysicalAttackPower + mValue);
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

void DeterminationOfKnight::Render(HDC hdc)
{
	CAMERA->ScaleRender(hdc, mImage, mRect.left, mRect.top, mSizeX, mSizeY);

	if (mIsCollision)
	{
		SetBkMode(hdc, TRANSPARENT);
		CallFont(hdc, 15, [&]()
		{
			TextOut(hdc, mRect.left - CAMERA->GetRect().left, mRect.top - 40 - CAMERA->GetRect().top, mItemName.c_str(), mItemName.size());
			TextOut(hdc, mRect.left - CAMERA->GetRect().left, mRect.top - 25 - CAMERA->GetRect().top, mExplanation.c_str(), mExplanation.size());
			TextOut(hdc, mRect.left - CAMERA->GetRect().left, mRect.top - 10 - CAMERA->GetRect().top, mEffect.c_str(), mEffect.size());
		});
		SetBkMode(hdc, OPAQUE);

	}

}

void DeterminationOfKnight::Release()
{

}
