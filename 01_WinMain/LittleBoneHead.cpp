#include "pch.h"
#include "LittleBoneHead.h"
#include "Inventory.h"
#include "Effect.h"


LittleBoneHead::LittleBoneHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "LittleBoneHead";

	mItemName = L"��Ʋ��";
	mExplanation = L"���ռ� ������ ���� ���̷���. �ٸ� ���̷��濡 ���� ü���� �۾� ��Ʋ���̶� �Ҹ���.";
	mEffect = L"\n���� �� ���� ��� ȸ���� ���� �������� �����ϴ�.";

	mSlot1Name = L"�ΰ��� ��ô";
	mSlot1Explanation = L"�ڽ��� �Ӹ��� ���� ���� �������� �����ϴ�. \n��ٿ� 4��.";
	
	mSlot2Name = L"�Ӹ��� ��ü";
	mSlot2Explanation =L"�Ӹ��� ���� ������ ��, �Ӹ��� �̵��մϴ�.";


	IMAGEMANAGER->LoadFromFile(L"LittleBoneHead", Resources(L"item/LittleBoneHead.bmp"), 22, 22, true);
	mImage = IMAGEMANAGER->FindImage(L"LittleBoneHead");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void LittleBoneHead::Update()
{
	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetRect();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration >= 0) //ȹ�� Ʈ����
		{
			SKUL->GetInventory()->GetSkul(this);
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

void LittleBoneHead::Render(HDC hdc)
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

void LittleBoneHead::Release()
{
}
