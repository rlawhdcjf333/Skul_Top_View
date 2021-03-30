#include "pch.h"
#include "GrimReaperHead.h"
#include "GrimReaper.h"
#include "Inventory.h"
#include "Effect.h"

GrimReaperHead::GrimReaperHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "GrimReaperHead";

	mItemName = L"�׸�����";
	mExplanation = L"������ �����ϴ� ����� ��.";
	mEffect = L"�� óġ �� ��ȥ���� ��Ÿ���ϴ�.\n��ȥ���� �ֺ��� �ٸ� ���� �����Ͽ� ������ ���� �������� �����ϴ�.\n\n������ �ε�\n���� �� �������·� ��Ÿ�� ���� �ֵѷ� ���� �������� �����ϴ�.";

	mSlot1Name = L"����";
	mSlot1Explanation = L"�����Ͽ� �����ڸ��� �ִ� ���� ������ ���� ���� �������� �����ϴ�.\n�ǰݵ� ������ �տ��� �߻��Ͽ� ���� �������� �����ϴ�.\n\n��ٿ� 12��.";

	mSlot2Name = L"��Ȯ";
	mSlot2Explanation = L"�������·� �ֺ� ������ ���� ��\n���� ũ�� �ֵѷ� ���� �������� �����ϴ�.\n\n��ٿ� 45��.";


	IMAGEMANAGER->LoadFromFile(L"GrimReaperHead", Resources(L"skulImage/GrimReaperHead.bmp"), 36, 24, true);
	IMAGEMANAGER->LoadFromFile(L"GrimReaperSkill1", Resources(L"skillImage/GrimReaperSkill1.bmp"), 24, 24, true);
	IMAGEMANAGER->LoadFromFile(L"GrimReaperSkill2", Resources(L"skillImage/GrimReaperSkill2.bmp"), 24, 24, true);

	mImage = IMAGEMANAGER->FindImage(L"GrimReaperHead");
	mSlot1Image = IMAGEMANAGER->FindImage(L"GrimReaperSkill1");
	mSlot2Image = IMAGEMANAGER->FindImage(L"GrimReaperSkill2");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void GrimReaperHead::Update()
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
			Player* input = new GrimReaper(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//���� ��������� ����
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

void GrimReaperHead::Render(HDC hdc)
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

void GrimReaperHead::Release()
{
}
