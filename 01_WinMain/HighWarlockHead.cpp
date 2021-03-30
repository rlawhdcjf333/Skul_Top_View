#include "pch.h"
#include "HighWarlockHead.h"
#include "HighWarlock.h"
#include "Inventory.h"
#include "Effect.h"

HighWarlockHead::HighWarlockHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "HighWarlockHead";

	mItemName = L"�븶����";
	mExplanation = L"������ �帧�� �����µ� �찡���� ���ذ� �� ���̴�.-���ռ� �� 1���� ������-";
	mEffect = L"��ų ��� �� ��ư�� ������ ������ ������ �����ϸ� �������� �ð��� ����Ͽ� ��ų�� ��ȭ�˴ϴ�.\n�������� �Ϸ� �� ��ų�� ���� �� ��ȭ�˴ϴ�.\n\n��� �ʵ�\n�Ŵ��� ��踦 �����մϴ�.��� �� ���鿡�� ���������� �������� ������ ��� ������ ���ĳ��ϴ�.";

	mSlot1Name = L"��� ���׿�";
	mSlot1Explanation = L"�ڽ��� �ֺ��� ��� �������ϴ�.\n�������� �ð��� ���� ��� ������ �����մϴ�.\n�������� �Ϸ� ��\n�Ŵ��� �ϳ��� ��� �������ϴ�.\n(���� ������)\n\n��ٿ� 20��.";

	mSlot2Name = L"��� ����";
	mSlot2Explanation = L"������ ���ư��� ���������� �������� ������ ��ü�� �����մϴ�.\n�������� �ð��� ���� ��ü�� ũ��� �������� �����մϴ�.\n�������� �Ϸ� �� ��ü�� ����� �� ������ ����ŵ�ϴ�.\n(���� ������)\n\n��ٿ� 12��.";
	mType = ItemType::SkulHead;


	IMAGEMANAGER->LoadFromFile(L"HighWarlockHead", Resources(L"skulImage/HighWarlockHead.bmp"), 26, 21, true);
	

	mImage = IMAGEMANAGER->FindImage(L"HighWarlockHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"HighWarlockSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"HighWarlockSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void HighWarlockHead::Update()
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
			Player* input = new HighWarlock(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//���� ��������� ����
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

void HighWarlockHead::Render(HDC hdc)
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

void HighWarlockHead::Release()
{
}
