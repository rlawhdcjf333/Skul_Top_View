#include "pch.h"
#include "HunterHead.h"
#include "Hunter.h"
#include "Inventory.h"
#include "Effect.h"

HunterHead::HunterHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "HunterHead";

	mItemName = L"����";
	mExplanation = L"Ȱ�� ȭ��� ������ �ǰ��� �ߴ� ���谡.";
	mEffect = L"���� ��ư�� ������ ������ ������ �����ϸ� �������� �Ϸ� �� ������ ��ȭ�˴ϴ�.\n\n�齺�� ��\n���� ��, �ڷ� �̵��Ͽ� ���� �����ϴ� ȭ���� �߻��� ������������ �����ϴ�.";

	mSlot1Name = L"��Ƽ�� ��";
	mSlot1Explanation = L"�������� �ټ��� ȭ���� �߻���\n������������ �����ϴ�. \n\n��ٿ� 8��.";

	mSlot2Name = L"�Ǿ ��";
	mSlot2Explanation = L"������ ���� ��մ� ����ȭ����\n�߻��� ������������ �����ϴ�.\n\n��ٿ� 14��.";

	mType = ItemType::SkulHead;

	IMAGEMANAGER->LoadFromFile(L"HunterHead", Resources(L"skulImage/HunterHead.bmp"), 27, 22, true);
	

	mImage = IMAGEMANAGER->FindImage(L"HunterHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"HunterSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"HunterSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void HunterHead::Update()
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
			Player* input = new Hunter(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//���� ��������� ����
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

void HunterHead::Render(HDC hdc)
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

void HunterHead::Release()
{
}
