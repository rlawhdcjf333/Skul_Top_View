#include "pch.h"
#include "EntHead.h"
#include "Inventory.h"
#include "Effect.h"
#include "Ent.h"


EntHead::EntHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "EntHead";

	mItemName = L"��Ʈ ����";
	mExplanation = L"��ȭ�� ���� �Ͽ��̾��� ���� ��Ʈ.";
	mEffect = L"�⺻���� �� 30% Ȯ���� �߰������� �߻��Ͽ� ������������ �����ϴ�.\n��Ŭ: �뽬 �� ��������, ���Ӵ뽬 �Ұ�.\n\nģȯ�� ����\n���� �� �������·� ���鿡 �����ִٰ� �ھƳ��� ������������ �����ϴ�.";

	mSlot1Name = L"���� ����";
	mSlot1Explanation = L"������ �������� ���������� ���ư�\n������������ �����ϴ�.\n\n��ٿ� 11��.";

	mSlot2Name = L"�޼���";
	mSlot2Explanation = L"�Ŵ��� �ָ����� ������� ������������ �����ϴ�.\n��ư�� ������ ������ ������ �����ϸ� ��������\n�Ϸ� �� ���ݹ����� �������� �����մϴ�.\n\n��ٿ� 13��.";


	IMAGEMANAGER->LoadFromFile(L"EntHead", Resources(L"skulImage/EntHead.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"EntSkill1", Resources(L"skillImage/EntSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"EntSkill2", Resources(L"skillImage/EntSkill2.bmp"), 24, 24, true);

	mImage = IMAGEMANAGER->FindImage(L"EntHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"EntSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"EntSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void EntHead::Update()
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
			Player* input = new Ent(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//���� ��������� ����
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

void EntHead::Render(HDC hdc)
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

void EntHead::Release()
{
}
