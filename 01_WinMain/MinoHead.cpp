#include "pch.h"
#include "MinoHead.h"
#include "Mino.h"
#include "Inventory.h"
#include "Effect.h"

MinoHead::MinoHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "MinoHead";

	mItemName = L"�̳�Ÿ��ν�";
	mExplanation = L"ũ��Ÿ �̱��� ��Ű�� ����.";
	mEffect = L"��ų ��� �� 3�ʰ� �ֺ� ������ ������������ �����ϴ�.\n��Ŭ: �뽬 �� ��������, ���Ӵ뽬 �Ұ�\n\nī�� ������\n���� ��, ������ ���������� ������������ �����ϴ�.";

	mSlot1Name = L"��ġ��";
	mSlot1Explanation = L"�����Ͽ� �ε��� ���� ����������\n������������ �����ϴ�. \n\n��ٿ� 10��.";

	mSlot2Name = L"�бⰭŸ";
	mSlot2Explanation = L"�����Ͽ� ������������ �����ϴ�.\n���������� ����Ǳ� ���� ��ư�� �� �� �� ���� �� �б�� �ķ��� ������������ ������ \n������ �ο��մϴ�.\n\n��ٿ� 14��.";


	IMAGEMANAGER->LoadFromFile(L"MinoHead", Resources(L"skulImage/MinotaurusHead.bmp"), 38, 35, true);
	IMAGEMANAGER->LoadFromFile(L"MinoSkill1", Resources(L"skillImage/MinoSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"MinoSkill2", Resources(L"skillImage/MinoSkill2.bmp"), 24, 24, true);

	mImage = IMAGEMANAGER->FindImage(L"MinoHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"MinoSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"MinoSkill2");
	mType = ItemType::SkulHead;

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	mType = ItemType::SkulHead;

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void MinoHead::Update()
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
			Player* input = new Mino(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//���� ��������� ����
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

void MinoHead::Render(HDC hdc)
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

void MinoHead::Release()
{
}
