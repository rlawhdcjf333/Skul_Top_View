#include "pch.h"
#include "TetanusBuff.h"
#include "Enemy.h"
#include "Bleeding.h"
#include "Animation.h"

TetanusBuff::TetanusBuff(GameObject* target, float duration)
{
	mName = "TetanusBuff";
	mIsActive = true;
	mIsDestroy = false;

	mTarget = target;
	mDuration = duration;

	mX = target->GetX();
	mY = target->GetY();

	IMAGEMANAGER->LoadFromFile(L"TetanusBuff", Resources(L"/skul/tetanusBuff.bmp"), 500, 400, 5, 4, true);
	mImage = IMAGEMANAGER->FindImage(L"TetanusBuff");

	mSizeX = 100;
	mSizeY = 100;
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

	mAnimation = new Animation(0, 0, 4, 3, false, true, 0.05f);
	mAnimation->Play();
	
	if (Obj->FindObject(ObjectLayer::Condition, "TetanusBuff")) //이미 존재하는 버프라면 갱신, 중첩버프로 사기칠 생각 ㄴ
	{
		Obj->FindObject(ObjectLayer::Condition, "TetanusBuff")->SetIsDestroy(true);
	}
	Obj->AddObject(ObjectLayer::Condition, this);
}

void TetanusBuff::Update()
{

	if (mDuration < 0 or !mTarget) { mIsDestroy = true; return; }
	else
	{
		mDuration -= dTime;

		mAnimation->Update();	
		mX = mTarget->GetX();
		mY = mTarget->GetY();
		mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

		for (GameObject* elem : Obj->GetObjectList(ObjectLayer::Enemy))
		{
			Enemy* downcast = (Enemy*)elem;
			if (downcast->GetHitTime() == 0.6f)
			{
				new Bleeding(elem, mMagicalAttackPower, 3);
			}
		}
	}


}

void TetanusBuff::Render(HDC hdc)
{
	CAMERA->AlphaScaleFrameRender(hdc, mImage, mRect.left, mRect.top-25, mAnimation->GetNowFrameX(), mAnimation->GetNowFrameY(), mSizeX, mSizeY, 0.5f);
}

void TetanusBuff::Release()
{
	SafeDelete(mAnimation)
}