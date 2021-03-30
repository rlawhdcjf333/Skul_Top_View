#include "pch.h"
#include "AlchemistHead.h"
#include "Alchemist.h"
#include "Inventory.h"
#include "Effect.h"

AlchemistHead::AlchemistHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "AlchemistHead";

	mItemName = L"���ݼ���";
	mExplanation = L"��ü ��𼭺��� �߸��� �ɱ�...";
	mEffect = L"10�ʸ��� �ҿ����� �������� ���� ��ȯ�մϴ�.(���� ������)\n\n��������\n���� ��, ��濡 �ö�ũ�� ��Ѹ��ϴ�.(���� ������)";

	mSlot1Name = L"���� �ö�ũ";
	mSlot1Explanation = L"������ �����ϴ� �ö�ũ�� �����ϴ�.\n(���� ������)\n�� ��ų�� 8ȸ ������ �� �ֽ��ϴ�.\n\n���� 4��.";

	mSlot2Name = L"ȭ�� �ö�ũ";
	mSlot2Explanation = L"������ �����ϴ� �ö�ũ�� �����ϴ�.\n(���� ������)\n�� ���� �� ȭ�� �ο�(ȭ��: ���� ������)\n�� ��ų�� 5ȸ ������ �� �ֽ��ϴ�.\n\n���� 13��.";

	mType = ItemType::SkulHead;

	IMAGEMANAGER->LoadFromFile(L"AlchemistHead", Resources(L"skulImage/AlchemistHead.bmp"), 26, 21, true);
		
	mImage = IMAGEMANAGER->FindImage(L"AlchemistHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"AlchemistSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"AlchemistSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void AlchemistHead::Update()
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
			Player* input = new Alchemist(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//���� ��������� ����
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

void AlchemistHead::Render(HDC hdc)
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

void AlchemistHead::Release()
{
}
