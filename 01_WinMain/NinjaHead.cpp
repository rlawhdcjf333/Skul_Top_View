#include "pch.h"
#include "NinjaHead.h"
#include "Ninja.h"
#include "Inventory.h"
#include "Effect.h"

NinjaHead::NinjaHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "NinjaHead";

	mItemName = L"����";
	mExplanation = L"������ �ϻ��� ������ ����.";
	mEffect = L"�� ������ 3�� �뽬�� �� �� �ֽ��ϴ�.\n\nȭ��: ȭ���� ����\n���� ��, �������� ���� �վ� ���� �������� ������ ȭ���� �ο��մϴ�.(���� ������)";

	mSlot1Name = L"ȯ������";
	mSlot1Explanation = L"�н��� ��ȯ�Ͽ� �Բ� ���� �������մϴ�.\n(���� ������)\n\n��ٿ� 8��.";

	mSlot2Name = L"Į������";
	mSlot2Explanation = L"�������� ������ �������� �����ϴ�.\n(���� ������)\n\n��ٿ� 7��.";


	IMAGEMANAGER->LoadFromFile(L"NinjaHead", Resources(L"skulImage/NinjaHead.bmp"), 30, 22, true);
	IMAGEMANAGER->LoadFromFile(L"NinjaSkill1", Resources(L"skillImage/NinjaSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"NinjaSkill2", Resources(L"skillImage/NinjaSkill2.bmp"), 24, 24, true);

	mImage = IMAGEMANAGER->FindImage(L"NinjaHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"NinjaSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"NinjaSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	mType = ItemType::SkulHead;

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void NinjaHead::Update()
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
			Player* input = new Ninja(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//���� ��������� ����
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

void NinjaHead::Render(HDC hdc)
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

void NinjaHead::Release()
{
}
