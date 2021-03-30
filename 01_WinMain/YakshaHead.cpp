#include "pch.h"
#include "YakshaHead.h"
#include "Yaksha.h"
#include "Inventory.h"
#include "Effect.h"

YakshaHead::YakshaHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "YakshaHead";

	mItemName = L"����";
	mExplanation = L"������ ����⸦ �ϻ�� �Ŵ��� ������.";
	mEffect = L"5��°�� ���� ���� �� ���濡 ū ���ظ� �����ϴ�.(���� ������)\n��Ŭ: �뽬 �� ��������, ���Ӵ뽬 �Ұ�";

	mSlot1Name = L"��������";
	mSlot1Explanation = L"��ų��ư�� ������ ������ ���������� �մϴ�.\n�������� �Ϸ�� ��ų�� ������ �����մϴ�.(���� ������)\n\n��ٿ� 12��.";

	mSlot2Name = L"��������";
	mSlot2Explanation = L"������ ���ư��� ���� 3�� �����ϴ�.(���� ������)\n\n��ٿ� 8��.";


	IMAGEMANAGER->LoadFromFile(L"YakshaHead", Resources(L"skulImage/YakshaHead.bmp"), 25, 25, true);
	

	mImage = IMAGEMANAGER->FindImage(L"YakshaHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"YakshaSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"YakshaSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;
	mType = ItemType::SkulHead;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void YakshaHead::Update()
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
			Player* input = new Yaksha(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//���� ��������� ����
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

void YakshaHead::Render(HDC hdc)
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

void YakshaHead::Release()
{
}
