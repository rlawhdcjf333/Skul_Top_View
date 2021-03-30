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

	mItemName = L"대마도사";
	mExplanation = L"마나의 흐름을 느끼는데 살가죽은 방해가 될 뿐이다.-마왕성 제 1마도 군단장-";
	mEffect = L"스킬 사용 시 버튼을 누르고 있으면 정신을 집중하며 정신집중 시간에 비례하여 스킬이 강화됩니다.\n정신집중 완료 시 스킬이 한층 더 강화됩니다.\n\n어비스 필드\n거대한 결계를 생성합니다.결계 내 적들에게 지속적으로 데미지를 입히고 결계 밖으로 밀쳐냅니다.";

	mSlot1Name = L"어비스 메테오";
	mSlot1Explanation = L"자신의 주변에 운석이 떨어집니다.\n정신집중 시간에 따라 운석의 개수가 증가합니다.\n정신집중 완료 시\n거대한 하나의 운석이 떨어집니다.\n(마법 데미지)\n\n쿨다운 20초.";

	mSlot2Name = L"어비스 오브";
	mSlot2Explanation = L"앞으로 나아가며 지속적으로 데미지를 입히는 구체를 생성합니다.\n정신집중 시간에 따라 구체의 크기와 데미지가 증가합니다.\n정신집중 완료 시 구체가 사라질 때 폭발을 일으킵니다.\n(마법 데미지)\n\n쿨다운 12초.";
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

		if (INPUT->GetKeyUp('F') and mDuration >= 1.8f) //획득 트리거
		{
			Player* input = new HighWarlock(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY(), 30, 30);//새로 만들어지는 스컬
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
