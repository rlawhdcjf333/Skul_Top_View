#include "pch.h"
#include "GoldenIncenseBurner.h"
#include "Inventory.h"
#include "Effect.h"

GoldenIncenseBurner::GoldenIncenseBurner(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "GoldenIncenseBurner";

	mItemName = L"Ȳ�����";
	mExplanation = L"�̰� �Ĵ� ���ε��� Ȳ�ݾ��� ���� ������ �踦 ���� ���̴�!";
	mEffect = L"ȹ���ϴ� ��差�� 20% �����մϴ�.";

	IMAGEMANAGER->LoadFromFile(L"Scruge", Resources(L"/item/Scruge.bmp"), 78, 78, true);
	mSlot1Name = L"������";
	mSlot1Image = IMAGEMANAGER->FindImage(L"Scruge");
	mSlot1Explanation = L"ȹ���ϴ� ��差�� 3/12/25% �����մϴ�.";

	IMAGEMANAGER->LoadFromFile(L"GoldenIncenseBurner", Resources(L"item/GoldenIncenseBurner1.bmp"), 20, 26, true);
	mImage = IMAGEMANAGER->FindImage(L"GoldenIncenseBurner");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mType = ItemType::CommonItem;

	mValue =20.f;
	mActivationFunc = []() {};
	mDeactivationFunc = []() {SKUL->SetGoldBonusRatio(SKUL->GetGoldBonusRatio() - 20.f);};
	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void GoldenIncenseBurner::Update()
{
	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetRect();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //ȹ�� Ʈ����
		{
			SKUL->GetInventory()->GetItem(this);
			SKUL->SetGoldBonusRatio(SKUL->GetGoldBonusRatio() + 20.f);
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

void GoldenIncenseBurner::Render(HDC hdc)
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

void GoldenIncenseBurner::Release()
{

}
