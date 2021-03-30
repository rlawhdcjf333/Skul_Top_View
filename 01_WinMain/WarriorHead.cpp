#include "pch.h"
#include "WarriorHead.h"
#include "Warrior.h"
#include "Inventory.h"
#include "Effect.h"

WarriorHead::WarriorHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "WarriorHead";

	mItemName = L"������";
	mExplanation = L"�ϸ�Ͼ� ��� ������ ���ֹ��� ��������� �� ������ �Ŀ�";
	mEffect = L"�ֺ��� �� ���� ����Ͽ� �������ݷ��� �ִ� 30%���� �����մϴ�.\n��Ŭ: �뽬 �� ��������, ���Ӵ뽬 �Ұ�\n\n�Ŀ�����\n���� ��, �������� �����ġ�� ������ ���մϴ�.(���� ������)";

	mSlot1Name = L"����¡ ������";
	mSlot1Explanation = L"ũ�� �÷����� ������ �������� �����ϴ�.\n(���� ������)\n\n��ٿ� 5��.";

	mSlot2Name = L"����";
	mSlot2Explanation = L"������� ������ � �� �ڽ��� �ڷ� �������� Ĩ�ϴ�.\n\n��ٿ� 5��.";


	IMAGEMANAGER->LoadFromFile(L"WarriorHead", Resources(L"skulImage/WarriorHead.bmp"), 19, 17, true);
	

	mImage = IMAGEMANAGER->FindImage(L"WarriorHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"WarriorSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"WarriorSkill2");
	mType = ItemType::SkulHead;

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void WarriorHead::Update()
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
			Player* input = new Warrior(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//���� ��������� ����
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

void WarriorHead::Render(HDC hdc)
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

void WarriorHead::Release()
{
}
