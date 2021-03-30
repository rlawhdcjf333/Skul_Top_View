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

	mItemName = L"닌자";
	mExplanation = L"동방의 암살자 집단의 수장.";
	mEffect = L"이 스컬은 3단 대쉬를 할 수 있습니다.\n\n화둔: 화룡의 숨결\n교대 시, 전방으로 불을 뿜어 마법 데미지를 입히고 화상을 부여합니다.(마법 데미지)";

	mSlot1Name = L"환영난무";
	mSlot1Explanation = L"분신을 소환하여 함께 적을 난도질합니다.\n(물리 데미지)\n\n쿨다운 8초.";

	mSlot2Name = L"칼날만개";
	mSlot2Explanation = L"전방으로 수많은 수리검을 날립니다.\n(물리 데미지)\n\n쿨다운 7초.";


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

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //획득 트리거
		{
			Player* input = new Ninja(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
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
