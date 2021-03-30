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

	mItemName = L"워리어";
	mExplanation = L"하모니아 대륙 북쪽의 저주받은 전사부족과 그 마지막 후예";
	mEffect = L"주변의 적 수에 비례하여 물리공격력이 최대 30%까지 증가합니다.\n태클: 대쉬 시 공격판정, 연속대쉬 불가\n\n파워차지\n교대 시, 전방으로 몸통박치기 공격을 가합니다.(물리 데미지)";

	mSlot1Name = L"라이징 슬래시";
	mSlot1Explanation = L"크게 올려베어 적들을 공중으로 날립니다.\n(물리 데미지)\n\n쿨다운 5초.";

	mSlot2Name = L"제압";
	mSlot2Explanation = L"대검으로 적들을 찌른 뒤 자신의 뒤로 내동댕이 칩니다.\n\n쿨다운 5초.";


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

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //획득 트리거
		{
			Player* input = new Warrior(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
			input->Init();
			Obj->AddObject(ObjectLayer::Player, input);//새 스컬을 오브젝트 매니저에 등록
			SKUL->GetInventory()->GetSkul(this); // 인벤토리에 이 스컬 머리를 등록
			SKUL->NewSkulGet(input);//새 스컬을 스컬매니저에 등록
			mIsTrashed = false;
			SetObjectOnTile(0, 0); //안보이는 어디론가로 숨겨놓는다... 이러면 어차피 클리핑되서 렌더도 안 돈다;
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

	if (mDuration < 0) //아이템 파괴 트리거
	{
		new Effect(L"Debris", mX, mY, EffectType::Normal);
		SetObjectOnTile(1, 1); // 적당히 어딘가 쳐박아 두기
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
