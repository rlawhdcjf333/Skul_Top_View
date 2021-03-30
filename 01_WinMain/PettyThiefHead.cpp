#include "pch.h"
#include "PettyThiefHead.h"
#include "PettyThief.h"
#include "Inventory.h"
#include "Effect.h"

PettyThiefHead::PettyThiefHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "PettyThiefHead";

	mItemName = L"������";
	mExplanation = L"���� �������� ������� ���� �ִ°� �ƴ϶� �ذ�� ��Ȱ�������� ����!";
	mEffect = L"������ �⺻ �������� �������� ���� �� 10% Ȯ���� 3 ��ȭ�� �������ϴ�.\n������: �뽬�Ÿ� ����.\n\n��Ÿ� ������\n���� ��, �ڷ� �̵��Ͽ� �������� ������������ �����ϴ�.";

	mSlot1Name = L"�ָӴ� ����";
	mSlot1Explanation = L"�������·� �ܰ��� �ֵѷ� ������������ �����ϴ�.\n�ǰݵ� �����κ��� 2 ��ȭ�� �������ϴ�.\n\n��ٿ� 8��.";

	mSlot2Name = L"�ڱ�����";
	mSlot2Explanation = L"���� ������ ������ ������������ ������\n�������·� �ڷ� ���� ���� ������ ȸ���մϴ�.\n�� ��ų�� �ִ� 3ȸ ������ �� �ֽ��ϴ�..\n\n��ٿ� 7��.";


	IMAGEMANAGER->LoadFromFile(L"PettyThiefHead", Resources(L"skulImage/PettyThiefHead.bmp"), 21, 21, true);
	IMAGEMANAGER->LoadFromFile(L"PettyThiefSkill1", Resources(L"skillImage/PettyThiefSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"PettyThiefSkill2", Resources(L"skillImage/PettyThiefSkill2.bmp"), 24, 24, true);

	mImage = IMAGEMANAGER->FindImage(L"PettyThiefHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"PettyThiefSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"PettyThiefSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void PettyThiefHead::Update()
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
			Player* input = new PettyThief(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//���� ��������� ����
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

void PettyThiefHead::Render(HDC hdc)
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

void PettyThiefHead::Release()
{
}
