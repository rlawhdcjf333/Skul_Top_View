#include "pch.h"
#include "Effect.h"
#include "Animation.h"

Effect::Effect(wstring keyname, float x, float y)
	:GameObject(),mNextImageKeyname(L""),mNextX(NULL),mNextY(NULL)
{
	mImage =  IMAGEMANAGER->FindImage(keyname);
	mX = x;
	mY = y;
	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mAnimation=new Animation();
	mAnimation->InitFrameByStartEnd(0,0,mImage->GetFrameX(),0,false);
	mAnimation->SetFrameUpdateTime(0.1f);
	mAnimation->Play();
	Obj->AddObject(ObjectLayer::Effect,this);
}

void Effect::Init()
{
}

void Effect::Update()
{
	if (!mImage) { //이미지가 없을 시
		mIsDestroy = true;
		return;
	}
	if (!mAnimation->GetIsPlay()) {
		if (mNextImageKeyname != L"") {
			if (mNextX != NULL && mNextY != NULL) {
				new Effect(mNextImageKeyname,mNextX, mNextY);
			}
			else {
				new Effect(mNextImageKeyname,mX,mY);
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
	CAMERA->Render(hdc,mImage,mX,mY);
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
