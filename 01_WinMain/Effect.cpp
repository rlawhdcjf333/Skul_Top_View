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
		mAnimation->InitFrameByStartEnd(0, 0, mImage->GetFrameX(), 0, true);
	}
	else
	{
		mAnimation->InitFrameByStartEnd(0,0,mImage->GetFrameX(),0,false);
	}
	mAnimation->SetCallbackFunc([this]() {mAnimation->SetCurrentFrameIndex(mImage->GetFrameX()-1);});
	mAnimation->SetFrameUpdateTime(0.1f);
	mAnimation->Play();
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

	if (!mImage) { //이미지가 없을 시
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
	CAMERA->FrameRender(hdc,mImage,mRect.left,mRect.top,mAnimation->GetNowFrameX(), 0);
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
