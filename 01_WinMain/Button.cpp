#include "pch.h"
#include "Button.h"

Button::Button(wstring text,float x, float y, float sizeX, float sizeY, function<void(void)> func)
{
	mText = text;
	mX = x;
	mY = y;
	mSizeX = sizeX;
	mSizeY = sizeY;
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mFunc = func;
	mState = State::Normal;
}

void Button::Update()
{
	if (mState == State::Normal)
	{
		if (Input::GetInstance()->GetKeyDown(VK_LBUTTON))
		{
			if (PtInRect(&mRect, _mousePosition))
			{
				mState = State::Push;
			}
		}
	}
	else
	{
		if (Input::GetInstance()->GetKeyUp(VK_LBUTTON))
		{
			mState = State::Normal;
			if (mFunc != nullptr)
			{
				mFunc();
			}
		}

	}
}

void Button::Render(HDC hdc)
{
	if (mState == State::Normal)
		RoundRect(hdc, mRect.left, mRect.top, mRect.right, mRect.bottom, mSizeX, mSizeY);
	else
	{
		float sizeX = mSizeX * 0.9f;
		float sizeY = mSizeY * 0.9f;
		RECT rc = RectMakeCenter(mX, mY, sizeX, sizeY);
		RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, mSizeX, mSizeY);
	}

	DrawText(hdc, mText.c_str(), mText.size(), &mRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}
