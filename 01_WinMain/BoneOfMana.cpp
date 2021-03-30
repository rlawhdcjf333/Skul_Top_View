#include "pch.h"
#include "BoneOfMana.h"
#include "Inventory.h"
#include "Effect.h"

BoneOfMana::BoneOfMana(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "BoneOfMana";

	mItemName = L"������ ��";
	mExplanation = L"��ü � ������ ���̷����� �̷��� ���� ����?";
	mEffect = L"��ų�� ���� �غ�ǰų� ������ ������ �ڽ� �ֺ��� ���� ȸ������ ����ŵ�ϴ�.";

	IMAGEMANAGER->LoadFromFile(L"BoneOfManaEffect", Resources(L"item/BoneOfManaEffect.bmp"), 500, 500, 5, 5, true);

	IMAGEMANAGER->LoadFromFile(L"Strategy", Resources(L"/item/Strategy.bmp"), 78, 78, true);
	mSlot1Name = L"����";
	mSlot1Image = IMAGEMANAGER->FindImage(L"Strategy");
	mSlot1Explanation = L"�������ݷ��� 15/25/40/100% �����˴ϴ�.";

	IMAGEMANAGER->LoadFromFile(L"BoneOfMana", Resources(L"item/BoneOfMana1.bmp"), 26, 26, true);
	mImage = IMAGEMANAGER->FindImage(L"BoneOfMana");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mType = ItemType::CommonItem;

	mValue = 0;
	mActivationFunc = []() 
	{
		if ((SKUL->GetCurrentSkul()->GetSkill1CoolTime() > 0 and SKUL->GetCurrentSkul()->GetSkill1CoolTime() <= dTime)
			or (SKUL->GetCurrentSkul()->GetSkill2CoolTime() > 0 and SKUL->GetCurrentSkul()->GetSkill2CoolTime() <= dTime))
		{
			SKUL->GetCurrentSkul()->Attack(mMagicalAttackPower, 2, AttackType::Whirlwind);
			(new Effect(L"BoneOfManaEffect", SKUL->GetCurrentSkul()->GetX(), SKUL->GetCurrentSkul()->GetY(), EffectType::Normal))->Scaling(200,200,0.5f);
		}
	};

	mDeactivationFunc = []() {};
	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void BoneOfMana::Update()
{
	RECT tmp = {};
	RECT skul = SKUL->GetCurrentSkul()->GetRect();
	if (IntersectRect(&tmp, &mRect, &skul))
	{
		mIsCollision = true;

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //ȹ�� Ʈ����
		{
			SKUL->GetInventory()->GetItem(this);
			mIsTrashed = false;
			SetObjectOnTile(0, 0); //�Ⱥ��̴� ���а��� ���ܳ��´�... �̷��� ������ Ŭ���εǼ� ������ �� ����;
			mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
		}

		if (INPUT->GetKey('F') and mDuration >= 0)
		{
			mDuration -= dTime;

			SetObjectOnTile(mIndexX, mIndexY);
			mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
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
		new Effect(L"Spark", mX, mY, EffectType::Normal);
		SetObjectOnTile(1, 1); // ������ ��� �Ĺھ� �α�
		mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	}
}

void BoneOfMana::Render(HDC hdc)
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

void BoneOfMana::Release()
{

}
