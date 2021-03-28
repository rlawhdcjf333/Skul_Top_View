#include "pch.h"
#include "Effect.h"
#include "Animation.h"

Effect::Effect(wstring keyname, float x, float y, EffectType type)
	:GameObject(),mNextImageKeyname(L""),mNextX(NULL),mNextY(NULL)
{
	mName.assign(keyname.begin(), keyname.end());
	mImage =  IMAGEMANAGER->FindImage(keyname);
	mX = x;
	mY = y;
	mType = type;
	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mAnimation=new Animation();
	if (type == EffectType::Reverse)
	{
		if (mImage->GetFrameY() != 0)
		{
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, mImage->GetFrameY() - 1, true);

		}
		else
		{
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX()-1, 0, true);
		}
	}
	else
	{
		if (mImage->GetFrameY() != 0)
		{
			mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX() - 1, mImage->GetFrameY()-1, false);

		}
		else
		{
			mAnimation->InitFrameByStartEnd(0,0,mImage->GetFrameX()-1,0,false);
		}

	}
	mAnimation->SetCallbackFunc([this]() {mAnimation->SetCurrentFrameIndex(mAnimation->GetFrameSize()-1);});
	mAnimation->SetFrameUpdateTime(0.05f);
	mAnimation->Play();
	mAlpha = 1.f;

	Obj->AddObject(ObjectLayer::Effect,this);
}

void Effect::Init()
{
}

void Effect::Update()
{

	if (mType == EffectType::Follow)
	{
		mX = SKUL->GetCurrentSkul()->GetX();
		mY = SKUL->GetCurrentSkul()->GetY()-15;
		mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	}

	if (!mImage) { //�̹����� ���� ��
		mIsDestroy = true;
		return;
	}
	if (!mAnimation->GetIsPlay()) {
		if (mNextImageKeyname != L"") {
			if (mNextX != NULL && mNextY != NULL) {
				new Effect(mNextImageKeyname,mNextX, mNextY, mType);
			}
			else {
				new Effect(mNextImageKeyname,mX,mY, mType);
			}
		}
		mIsDestroy = true;
	}
	else {
		mAnimation->Update();
	}
}

void Effect::Render(HDC hdc)
{
	if (!mImage) {
		return;
	}
	CAMERA->AlphaScaleFrameRender(hdc, mImage, mRect.left, mRect.top, mAnimation->GetNowFrameX(), mAnimation->GetNowFrameY(), mSizeX, mSizeY, mAlpha);
}

void Effect::Release()
{
	SafeDelete(mAnimation)
}

void Effect::SetNextEffect(wstring keyname)
{	
	mNextImageKeyname = keyname;
}

void Effect::SetNextEffect(wstring keyname, float x, float y)
{
	mNextImageKeyname = keyname;
	mNextX = x;
	mNextY = y;
}

void Effect::SetUpdateTime(float val)
{
	 mAnimation->SetFrameUpdateTime(val); 
}
