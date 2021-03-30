#include "pch.h"
#include "LittleBoneHead.h"
#include "Inventory.h"
#include "Effect.h"
#include "LittleBone.h"


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
	mSlot1Explanation = L"�ڽ��� �Ӹ��� ���� ���� �������� �����ϴ�. \n\n��ٿ� 4��.";
	
	mSlot2Name = L"�Ӹ��� ��ü";
	mSlot2Explanation =L"�Ӹ��� ���� ������ ��, �Ӹ��� �̵��մϴ�.";

	mType = ItemType::SkulHead;

	IMAGEMANAGER->LoadFromFile(L"LittleBoneHead", Resources(L"skulImage/LittleBoneHead.bmp"), 15, 13, true);
	IMAGEMANAGER->LoadFromFile(L"LittleBoneSkill1", Resources(L"skillImage/LittleBone1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"LittleBoneSkill2", Resources(L"skillImage/LittleBone2.bmp"), 24, 24, true);
	mImage = IMAGEMANAGER->FindImage(L"LittleBoneHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"LittleBoneSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"LittleBoneSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void LittleBoneHead::Update()
{
	mDelay -= dTime;
	if (mDelay <= 0)mDelay = 0;
	if (mDelay > 0) return;

	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetHitBox();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //ȹ�� Ʈ����
		{
			Player* input = new LittleBone(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//���� ��������� ����
			input->Init();
			Obj->AddObject(ObjectLayer::Player, input);//�� ������ ������Ʈ �Ŵ����� ���
			SKUL->GetInventory()->GetSkul(this); // �κ��丮�� �� ���� �Ӹ��� ���
			SKUL->NewSkulGet(input);//�� ������ ���øŴ����� ���
			mIsTrashed = false;
			SetObjectOnTile(0, 0); //�Ⱥ��̴� ���а��� ���ܳ��´�... �̷��� ������ Ŭ���εǼ� ������ �� ����;
		}

		if (INPUT->GetKey('F'))
		{
			mDuration -= dTime;

			SetObjectOnTile(mIndexX, mIndexY);
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
		new Effect(L"Debris", mX, mY, EffectType::Normal);
		SetObjectOnTile(1, 1); // ������ ��� �Ĺھ� �α�
	}
}

void LittleBoneHead::Render(HDC hdc)
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

void LittleBoneHead::Release()
{
}
