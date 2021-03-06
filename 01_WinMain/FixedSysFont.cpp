#include "pch.h"
#include "FixedSysFont.h"

FixedSysFont::FixedSysFont(int x, int y, int sizeX, int sizeY, wstring num, FontColor color, bool Camera)
	:GameObject("FixedSysFont"), mNum(num), mColor(color), mTime(0)
{
	mX = x;
	mY = y - 20.f;
	mStartX = x;
	mStartY = y - 20.f;
	mSizeX = sizeX;
	mSizeY = sizeY;
	srand(time(NULL) + (int)mX);
	mXcount = RAND->RandomInt(-20, 20) / 50.f;
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	Obj->AddObject(ObjectLayer::Font,this);
}

void FixedSysFont::Init()
{
}

void FixedSysFont::Update()
{
	mTime += dTime;
	if (mTime < 0.4f) {
		mY -= 0.2;
	}
	else {
		mY += 0.2;
	}
	mX += mXcount;
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	if (mTime > 0.6f) {
		mIsDestroy = true;
	}
}

void FixedSysFont::Render(HDC hdc)
{
	FixedCamerRender(hdc);
}

void FixedSysFont::Release()
{
}

void FixedSysFont::FixedRender(HDC hdc) {
	HFONT Font = CreateFont(20,0,0,0,0,0,0,0,HANGEUL_CHARSET,0,0,0,VARIABLE_PITCH | FF_ROMAN, TEXT("FixedSys"));
	HFONT oldFont = (HFONT)SelectObject(hdc, Font);
	RECT area = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, mNum.c_str(), mNum.size(), &area, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOCLIP);
	SetBkMode(hdc, OPAQUE);
	SelectObject(hdc, oldFont);
	SetTextColor(hdc, RGB(0, 0, 0));
	DeleteObject(Font);
}

void FixedSysFont::FixedCamerRender(HDC hdc)
{
	HFONT Font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("FixedSys"));
	HFONT oldFont = (HFONT)SelectObject(hdc, Font);
	if (mColor == FontColor::Red) {
		SetTextColor(hdc, RGB(255, 0, 0));
	}
	else if (mColor == FontColor::Blue) {
		SetTextColor(hdc, RGB(0, 0, 255));
	}
	int x = mX - CAMERA->GetRect().left;
	int y = mY - CAMERA->GetRect().top;
	RECT area = RectMakeBottom(x, y, mSizeX, mSizeY);
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, mNum.c_str(), mNum.size(), &area, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOCLIP);
	SetBkMode(hdc, OPAQUE);
	SelectObject(hdc, oldFont);
	SetTextColor(hdc,RGB(0,0,0));
	DeleteObject(Font);
}
