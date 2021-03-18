#include "pch.h"
#include "Camera.h"
#include "Tile.h"
#include "Image.h"

void Camera::Init()
{
	mMode = Mode::Follow;
	//mTarget = nullptr;
	//mX = WINSIZEX / 2;
	//mY = WINSIZEY / 2;
	mSizeX = WINSIZEX;
	mSizeY = WINSIZEY;
	mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
	mMoveSpeed = 5.f;
}

void Camera::Release()
{
	//얘도 쓸일이 없다 . . . 
}

void Camera::Update()
{
	switch (mMode)
	{
	case Camera::Mode::Follow:
		if (mTarget)
		{
			//mX = mTarget->GetX();
			//mY = mTarget->GetY();
			//멀리있으면 빨리 쫓아가야하고 가까이 있으면 천천히 쫓아가야함
			mX = Math::Lerp(mX, mTarget->GetX(), 2.f * Time::GetInstance()->DeltaTime());
			mY = Math::Lerp(mY, mTarget->GetY(), 2.f * Time::GetInstance()->DeltaTime());

			mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
		}

		if (mRect.top < TILE[0][41]->GetY()) mY = TILE[0][41]->GetY() + mSizeY/2;
		if (mRect.right > TILE[0][42]->GetX()) mX = TILE[0][42]->GetX() - mSizeX / 2;
		if (mRect.left < TILE[40][0]->GetX()) mX = TILE[40][0]->GetX() + mSizeX / 2;
		if (mRect.bottom > TILE[36][74]->GetY()) mY = TILE[36][74]->GetY() - mSizeY / 2;
		
		mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

		break;
	case Camera::Mode::Free:
		if (Input::GetInstance()->GetKey('A'))mX -= mMoveSpeed;
		if (Input::GetInstance()->GetKey('D'))mX += mMoveSpeed;
		if (Input::GetInstance()->GetKey('W'))mY -= mMoveSpeed;
		if (Input::GetInstance()->GetKey('S'))mY += mMoveSpeed;
		mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);
		break;
	}

	Panning();
}

void Camera::Render(HDC hdc)
{
	//쓸일이 없다 . . . . 
}

void Camera::Render(HDC hdc, Image * image, int x, int y)
{
	if (mRect.left> x+image->GetWidth() || mRect.right < x) {
		return;
	}
	if (mRect.top> y+image->GetHeight() || mRect.bottom <y) {
		return;
	}
	image->Render(hdc, x - mRect.left, y - mRect.top);
}

void Camera::Render(HDC hdc, Image * image, int x, int y, int tempX, int tempY, int tempWidth, int tempHeight)
{
	if (mRect.left > x + tempWidth || mRect.right < x) {
		return;
	}
	if (mRect.top > y + tempHeight || mRect.bottom < y) {
		return;
	}
	image->Render(hdc, x - mRect.left, y - mRect.top, tempX, tempY, tempWidth, tempHeight);
}

void Camera::FrameRender(HDC hdc, Image * image, int x, int y, int frameX, int frameY)
{
	if (mRect.left > x + image->GetFrameWidth() || mRect.right < x) {
		return;
	}
	if (mRect.top > y + image->GetFrameHeight() || mRect.bottom < y) {
		return;
	}
	image->FrameRender(hdc, x - mRect.left, y - mRect.top, frameX, frameY);
}

void Camera::AlphaRender(HDC hdc, Image * image, int x, int y, float alpha)
{
	if (mRect.left > x + image->GetWidth() || mRect.right < x) {
		return;
	}
	if (mRect.top > y + image->GetHeight() || mRect.bottom < y) {
		return;
	}
	image->AlphaRender(hdc, x - mRect.left, y - mRect.top, alpha);
}

void Camera::AlphaFrameRender(HDC hdc, Image * image, int x, int y, int frameX, int frameY, float alpha)
{
	if (mRect.left > x + image->GetFrameWidth() || mRect.right < x) {
		return;
	}
	if (mRect.top > y + image->GetFrameHeight() || mRect.bottom < y) {
		return;
	}
	image->AlphaFrameRender(hdc, x - mRect.left, y - mRect.top, frameX, frameY, alpha);
}

void Camera::ScaleRender(HDC hdc, Image * image, int x, int y, int width, int height)
{
	if (mRect.left > x + width || mRect.right < x) {
		return;
	}
	if (mRect.top > y + height || mRect.bottom < y) {
		return;
	}
	image->ScaleRender(hdc, x - mRect.left, y - mRect.top, width, height);
}

void Camera::ScaleFrameRender(HDC hdc, Image * image, int x, int y, int frameX, int frameY, int width, int height)
{
	if (mRect.left > x + width || mRect.right < x) {
		return;
	}
	if (mRect.top > y + height || mRect.bottom < y) {
		return;
	}
	image->ScaleFrameRender(hdc, x - mRect.left, y - mRect.top, frameX, frameY, width, height);
}

void Camera::AlphaScaleRender(HDC hdc, Image * image, int x, int y, int width, int height, float alpha)
{
	if (mRect.left > x + width || mRect.right < x) {
		return;
	}
	if (mRect.top > y + height || mRect.bottom < y) {
		return;
	}
	image->AlphaScaleRender(hdc, x - mRect.left, y - mRect.top, width, height, alpha);
}

void Camera::AlphaScaleFrameRender(HDC hdc, Image * image, int x, int y, int frameX, int frameY, int width, int height, float alpha)
{
	if (mRect.left > x + width || mRect.right < x) {
		return;
	}
	if (mRect.top > y + height || mRect.bottom < y) {
		return;
	}
	image->AlphaScaleFrameRender(hdc, x - mRect.left, y - mRect.top, frameX, frameY, width, height, alpha);
}

void Camera::RenderRect(HDC hdc, RECT rc)
{
	rc.left -= mRect.left;
	rc.right -= mRect.left;
	rc.top -= mRect.top;
	rc.bottom -= mRect.top;
	::RenderRect(hdc, rc);
}

void Camera::RenderPolygon(HDC hdc, POINT points[], int pointsNum)
{
	for (int i = 0; i < pointsNum; i++)
	{
		points[i].x -= mRect.left;
		points[i].y -= mRect.top;
	}
	Polygon(hdc, points, pointsNum);
}

void Camera::RenderEllipse(HDC hdc, float x, float y, float radius)
{
	RenderEllipse(hdc, x - mRect.left, y - mRect.top, radius);
}

bool Camera::IsInCameraArea(float x, float y, float width, float height)
{
	RECT rc = RectMakeCenter(x, y, width, height);
	return IsInCameraArea(rc);
}

bool Camera::IsInCameraArea(RECT rc)
{
	if (rc.right < mRect.left)return false;
	if (rc.left > mRect.right)return false;
	if (rc.bottom < mRect.top)return false;
	if (rc.top > mRect.bottom)return false;

	return true;
}


void Camera::Panning()
{
	if (mIsPanning)
	{
		mPanningTime -= dTime;

		mX += mPanningPower * cosf(Random::GetInstance()->RandomInt(360) * PI2 / 360);
		mY -= mPanningPower * sinf(Random::GetInstance()->RandomInt(360) * PI2 / 360);
		mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY);

		if (mPanningTime < 0) { mPanningTime = 0.5f; mIsPanning = false; }
	}
}

LONG Camera::CameraMouseX() { return nonC_mousePosition.x + mRect.left; }
LONG Camera::CameraMouseY() { return nonC_mousePosition.y + mRect.top; }