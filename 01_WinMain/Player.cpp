#include "pch.h"
#include "Player.h"

Player::Player(float x, float y, float sizeX, float sizeY)
	:GameObject("Player"), mTime(0), mFrameX(0)  
{
	mX = x;
	mY = y;
	mSizeX = sizeX;
	mSizeY = sizeY;
}

void Player::Init()
{
	IMAGEMANAGER->LoadFromFile(L"Sans", Resources(L"Sans.bmp"), 92, 30, 4, 1, true);
	mImage = IMAGEMANAGER->GetInstance()->FindImage(L"Sans");

}

void Player::Update()
{
	mTime+=Time::GetInstance()->DeltaTime();

	if (mTime > 0.2)
	{
		mTime = 0;
		mFrameX++;
	}
	if (mFrameX > 3) mFrameX = 0;

	if (Input::GetInstance()->GetKey('D'))
	{
		mX += 30*Time::GetInstance()->DeltaTime(); mY += 15*Time::GetInstance()->DeltaTime();
	}
	if (Input::GetInstance()->GetKey('W'))
	{
		mX += 30 * Time::GetInstance()->DeltaTime(); mY -= 15 * Time::GetInstance()->DeltaTime();
	}
	if (Input::GetInstance()->GetKey('A'))
	{
		mX -= 30 * Time::GetInstance()->DeltaTime(); mY -= 15 * Time::GetInstance()->DeltaTime();
	}
	if (Input::GetInstance()->GetKey('S'))
	{
		mX -= 30 * Time::GetInstance()->DeltaTime(); mY += 15 * Time::GetInstance()->DeltaTime();
	}
}

void Player::Release()
{
}

void Player::Render(HDC hdc)
{
	mImage->ScaleFrameRender(hdc, mX-mSizeX/2, mY-mSizeY, mFrameX, 0, mSizeX, mSizeY);
}
