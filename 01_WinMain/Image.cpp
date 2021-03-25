#include "pch.h"
#include "Image.h"

#pragma comment(lib, "msimg32.lib")

Image::Image()
	:mImageBuffer(nullptr),mIsTrans(false),mBlendFunc(nullptr), mBlendImageBuffer(nullptr)
{
}


Image::~Image()
{
	ReleaseBuffer();
}

void Image::ReleaseBuffer()
{
	if (mImageBuffer != nullptr)
	{
		SelectObject(mImageBuffer->hdc, mImageBuffer->oldBitmap);
		DeleteObject(mImageBuffer->bitmap);
		DeleteDC(mImageBuffer->hdc);

		SelectObject(mBlendImageBuffer->hdc, mBlendImageBuffer->oldBitmap);
		DeleteObject(mBlendImageBuffer->bitmap);
		DeleteDC(mBlendImageBuffer->hdc);

		SafeDelete(mImageBuffer);
		SafeDelete(mBlendImageBuffer);
	}
}

bool Image::CreateEmpty(int width, int height)
{
	//이미 이미지 버퍼가 있다면 삭제
	if (mImageBuffer != nullptr)
	{
		ReleaseBuffer();
	}

	HDC hdc = GetDC(_hWnd);

	mImageBuffer = new ImageBuffer();
	mImageBuffer->loadType = LoadType::Empty;
	mImageBuffer->registerID = 0;
	mImageBuffer->hdc = CreateCompatibleDC(hdc);	//빈 HDC생성
	mImageBuffer->bitmap = (HBITMAP)CreateCompatibleBitmap(hdc, width, height); //빈 비트맵 생성
	mImageBuffer->oldBitmap = (HBITMAP)SelectObject(mImageBuffer->hdc, mImageBuffer->bitmap);	//이전 비트맵 저장
	mImageBuffer->width = width;
	mImageBuffer->height = height;

	mBlendFunc = new BLENDFUNCTION();
	mBlendFunc->BlendFlags = 0;
	mBlendFunc->AlphaFormat = 0;
	mBlendFunc->BlendOp = AC_SRC_OVER;

	mBlendImageBuffer = new ImageBuffer();
	mBlendImageBuffer->loadType = LoadType::Empty;
	mBlendImageBuffer->registerID = 0;
	mBlendImageBuffer->hdc = CreateCompatibleDC(hdc);
	mBlendImageBuffer->bitmap = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	mBlendImageBuffer->oldBitmap = (HBITMAP)SelectObject(mBlendImageBuffer->hdc, mBlendImageBuffer->bitmap);
	mBlendImageBuffer->width = WINSIZEX;
	mBlendImageBuffer->height = WINSIZEY;

	if (mImageBuffer->bitmap == nullptr)
	{
		ReleaseBuffer();
		return false;
	}

	ReleaseDC(_hWnd, hdc);

	return true;
}

bool Image::LoadFromFile(wstring keyName,wstring fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	if (fileName.empty() == true)
	{
		return false;
	}

	if (mImageBuffer != nullptr)
	{
		ReleaseBuffer();
	}

	HDC hdc = GetDC(_hWnd);

	mImageBuffer = new ImageBuffer();
	mImageBuffer->loadType = LoadType::File;
	mImageBuffer->registerID = 0;
	mImageBuffer->hdc = CreateCompatibleDC(hdc);
	mImageBuffer->bitmap = (HBITMAP)LoadImage(_hInstance, fileName.c_str(), IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	mImageBuffer->oldBitmap = (HBITMAP)SelectObject(mImageBuffer->hdc, mImageBuffer->bitmap);
	mImageBuffer->width = width;
	mImageBuffer->height = height;
	mImageBuffer->frameWidth = width;
	mImageBuffer->frameHeight = height;

	mBlendFunc = new BLENDFUNCTION();
	mBlendFunc->BlendFlags = 0;
	mBlendFunc->AlphaFormat = 0;
	mBlendFunc->BlendOp = AC_SRC_OVER;

	mBlendImageBuffer = new ImageBuffer();
	mBlendImageBuffer->loadType = LoadType::Empty;
	mBlendImageBuffer->registerID = 0;
	mBlendImageBuffer->hdc = CreateCompatibleDC(hdc);
	mBlendImageBuffer->bitmap = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	mBlendImageBuffer->oldBitmap = (HBITMAP)SelectObject(mBlendImageBuffer->hdc, mBlendImageBuffer->bitmap);
	mBlendImageBuffer->width = WINSIZEX;
	mBlendImageBuffer->height = WINSIZEY;

	mFileName = fileName;
	mKeyName = keyName;

	mIsTrans = isTrans;
	mTransColor = transColor;

	if (mImageBuffer->bitmap == nullptr)
	{
		ReleaseBuffer();

		return false;
	}

	ReleaseDC(_hWnd, hdc);

	return true;
}

bool Image::LoadFromFile(wstring keyName, wstring fileName, int width, int height, int frameX, int frameY, bool isTrans, COLORREF transColor)
{
	if (fileName.empty() == true)
	{
		return false;
	}

	if (mImageBuffer != nullptr)
	{
		ReleaseBuffer();
	}

	HDC hdc = GetDC(_hWnd);

	mImageBuffer = new ImageBuffer();
	mImageBuffer->loadType = LoadType::File;
	mImageBuffer->registerID = 0;
	mImageBuffer->hdc = CreateCompatibleDC(hdc);
	mImageBuffer->bitmap = (HBITMAP)LoadImage(_hInstance, fileName.c_str(), IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	mImageBuffer->oldBitmap = (HBITMAP)SelectObject(mImageBuffer->hdc, mImageBuffer->bitmap);
	mImageBuffer->width = width;
	mImageBuffer->height = height;
	mImageBuffer->frameX = frameX;
	mImageBuffer->frameY = frameY;
	mImageBuffer->frameWidth = width / frameX;
	mImageBuffer->frameHeight = height / frameY;

	mBlendFunc = new BLENDFUNCTION();
	mBlendFunc->BlendFlags = 0;
	mBlendFunc->AlphaFormat = 0;
	mBlendFunc->BlendOp = AC_SRC_OVER;

	mBlendImageBuffer = new ImageBuffer();
	mBlendImageBuffer->loadType = LoadType::Empty;
	mBlendImageBuffer->registerID = 0;
	mBlendImageBuffer->hdc = CreateCompatibleDC(hdc);
	mBlendImageBuffer->bitmap = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZEX, WINSIZEY);
	mBlendImageBuffer->oldBitmap = (HBITMAP)SelectObject(mBlendImageBuffer->hdc, mBlendImageBuffer->bitmap);
	mBlendImageBuffer->width = WINSIZEX;
	mBlendImageBuffer->height = WINSIZEY;

	mFileName = fileName;
	mKeyName = keyName;
	mIsTrans = isTrans;
	mTransColor = transColor;

	if (mImageBuffer->bitmap == nullptr)
	{
		ReleaseBuffer();

		return false;
	}

	ReleaseDC(_hWnd, hdc);

	return true;
}


/*
## Render ##
hdc : 그릴 HDC
x : 그릴 좌표X
y : 그릴 좌표Y
*/
void Image::Render(HDC hdc, int x, int y)
{
	if (mIsTrans)
	{
		GdiTransparentBlt(
			hdc,					//그릴 버퍼(HDC)
			x,						//그릴 좌표X
			y,						//그릴 좌표Y
			mImageBuffer->width,	//그릴 가로길이
			mImageBuffer->height,	//그릴 세로길이
			mImageBuffer->hdc,		//그릴 HDC
			0,						//복사해올 버퍼 시작좌표X
			0,						//복사해올 버퍼 시작좌표Y
			mImageBuffer->width,	//복사해올 버퍼 가로길이
			mImageBuffer->height,	//복사해올 버퍼 세로길이
			mTransColor				//Trans처리할 색상
		);
	}
	else
	{
		//HDC의 버퍼를 다른 HDC버퍼에 고속 복사해주는 함수
		BitBlt(hdc, x, y, mImageBuffer->width, mImageBuffer->height, mImageBuffer->hdc, 0, 0, SRCCOPY);
	}
}

/*
## Render ##
hdc : 그릴 HDC
x : 그릴 좌표
y : 그릴 좌표
tempX : 버퍼(텍스쳐)로부터 그릴 시작픽셀
tempY : 버퍼(텍스쳐)로부터 그릴 시작픽셀
tempWidth : 버퍼의 tempX로부터 그릴 너비
tempHeight : 버퍼의 tempY로부터 그릴 높이
*/

void Image::Render(HDC hdc, int x, int y, 
	int tempX, int tempY, int tempWidth, int tempHeight)
{
	if (mIsTrans)
	{
		GdiTransparentBlt
		(
			hdc,
			x,
			y,
			tempWidth,
			tempHeight,
			mImageBuffer->hdc,
			tempX,
			tempY,
			tempWidth,
			tempHeight,
			mTransColor
		);
	}
	else
	{
		BitBlt
		(
			hdc, x, y,
			tempWidth,
			tempHeight,
			mImageBuffer->hdc,
			tempX,
			tempY,
			SRCCOPY
		);
	}
}

/*
## FrameRender ##
x : 그릴 좌표
y : 그릴 좌표
frameX : 출력할 가로 프레임 인덱스
frameY : 출력할 세로 프레임 인덱스 
*/
void Image::FrameRender(HDC hdc, int x, int y, int frameX, int frameY)
{
	if (mIsTrans)
	{
		GdiTransparentBlt
		(
			hdc,
			x,
			y,
			mImageBuffer->frameWidth,
			mImageBuffer->frameHeight,
			mImageBuffer->hdc,
			mImageBuffer->frameWidth * frameX,
			mImageBuffer->frameHeight * frameY,
			mImageBuffer->frameWidth,
			mImageBuffer->frameHeight,
			mTransColor
		);
	}
	else
	{
		BitBlt
		(
			hdc,
			x,
			y,
			mImageBuffer->frameWidth,
			mImageBuffer->frameHeight,
			mImageBuffer->hdc,
			mImageBuffer->frameWidth * frameX,
			mImageBuffer->frameHeight * frameY,
			SRCCOPY
		);
	}
}

void Image::AlphaRender(HDC hdc,int x, int y, float alpha)
{
	mBlendFunc->SourceConstantAlpha = (BYTE)(alpha * 255.f);

	if (mIsTrans)
	{
		BitBlt(mBlendImageBuffer->hdc, 0, 0, mImageBuffer->width, mImageBuffer->height,
			hdc, x,y, SRCCOPY);

		GdiTransparentBlt(mBlendImageBuffer->hdc, 0, 0, mImageBuffer->width, mImageBuffer->height,
			mImageBuffer->hdc, 0, 0, mImageBuffer->width, mImageBuffer->height, mTransColor);

		AlphaBlend(hdc, x,y, mImageBuffer->width, mImageBuffer->height,
			mBlendImageBuffer->hdc, 0, 0, mImageBuffer->width, mImageBuffer->height, *mBlendFunc);
	}
	else
	{
		AlphaBlend(hdc, x, y, mImageBuffer->width, mImageBuffer->height,
			mImageBuffer->hdc, 0, 0, mImageBuffer->width, mImageBuffer->height, *mBlendFunc);
	}
}

void Image::AlphaFrameRender(HDC hdc, int x, int y, int frameX, int frameY, float alpha)
{
	mBlendFunc->SourceConstantAlpha = (BYTE)(alpha * 255.f);

	if (mIsTrans)
	{
		BitBlt(mBlendImageBuffer->hdc, 0, 0, mImageBuffer->frameWidth, mImageBuffer->frameHeight,
			hdc, x, y, SRCCOPY);

		GdiTransparentBlt(mBlendImageBuffer->hdc, 0, 0, mImageBuffer->frameWidth, mImageBuffer->frameHeight,
			mImageBuffer->hdc, mImageBuffer->frameWidth * frameX, mImageBuffer->frameHeight * frameY, 
			mImageBuffer->frameWidth, mImageBuffer->frameHeight, mTransColor);

		AlphaBlend(hdc, x, y, mImageBuffer->frameWidth, mImageBuffer->frameHeight,
			mBlendImageBuffer->hdc, 0,0,
			mImageBuffer->frameWidth, mImageBuffer->frameHeight, *mBlendFunc);
	}
	else
	{
		AlphaBlend(hdc, x, y, mImageBuffer->frameWidth, mImageBuffer->frameHeight,
			mImageBuffer->hdc, 0, 0, mImageBuffer->frameWidth, mImageBuffer->frameHeight, *mBlendFunc);
	}
}

void Image::ScaleRender(HDC hdc, int x, int y, int width, int height)
{
	if (mIsTrans)
	{
		GdiTransparentBlt(hdc, x, y, width, height,
			mImageBuffer->hdc, 0, 0, mImageBuffer->width, mImageBuffer->height,
			mTransColor);
	}
	else
	{
		StretchBlt(hdc, x, y, width, height, mImageBuffer->hdc, 0, 0, mImageBuffer->width, mImageBuffer->height,
			SRCCOPY);
	}
}

void Image::ScaleFrameRender(HDC hdc, int x, int y, int frameX, int frameY, int width, int height)
{
	if (mIsTrans)
	{
		GdiTransparentBlt(hdc, x, y, width, height,
			mImageBuffer->hdc, mImageBuffer->frameWidth * frameX, mImageBuffer->frameHeight * frameY,
			mImageBuffer->frameWidth, mImageBuffer->frameHeight, mTransColor);
	}
	else
	{
		BitBlt(hdc, x, y, width, height, mImageBuffer->hdc, mImageBuffer->frameWidth * frameX,
			mImageBuffer->frameHeight * frameY, SRCCOPY);
	}
}

void Image::AlphaScaleRender(HDC hdc, int x, int y, int width, int height, float alpha)
{
	mBlendFunc->SourceConstantAlpha = (BYTE)(alpha * 255.0f);

	if (mIsTrans)
	{
		BitBlt(mBlendImageBuffer->hdc, 0, 0,
			width, height,
			hdc, x, y, SRCCOPY);

		GdiTransparentBlt(mBlendImageBuffer->hdc, 0, 0, width, height,
			mImageBuffer->hdc,
			0, 0,
			mImageBuffer->width, mImageBuffer->height,
			mTransColor);

		AlphaBlend(hdc, x, y, width, height,
			mBlendImageBuffer->hdc,
			0, 0,
			width, height,
			*mBlendFunc);
	}
	else
	{
		AlphaBlend
		(
			hdc, x, y, width, height,
			mImageBuffer->hdc,
			0,0,
			mImageBuffer->width, mImageBuffer->frameHeight,
			*mBlendFunc
		);
	}
}

void Image::AlphaScaleFrameRender(HDC hdc, int x, int y, int frameX, int frameY, int width, int height, float alpha)
{
	mBlendFunc->SourceConstantAlpha = (BYTE)(alpha * 255.0f);

	if (mIsTrans)
	{
		BitBlt(mBlendImageBuffer->hdc, 0, 0,
			width, height,
			hdc, x, y, SRCCOPY);

		GdiTransparentBlt(mBlendImageBuffer->hdc, 0, 0, width, height,
			mImageBuffer->hdc,
			frameX * mImageBuffer->frameWidth, frameY * mImageBuffer->frameHeight,
			mImageBuffer->frameWidth, mImageBuffer->frameHeight,
			mTransColor);

		AlphaBlend(hdc, x, y, width, height,
			mBlendImageBuffer->hdc,
			0, 0,
			width, height,
			*mBlendFunc);
	}
	else
	{
		AlphaBlend
		(
			hdc, x, y, width, height,
			mImageBuffer->hdc, 
			mImageBuffer->frameWidth * frameX, mImageBuffer->frameHeight * frameY, 
			mImageBuffer->frameWidth, mImageBuffer->frameHeight,
			*mBlendFunc
		);
	}
}

void Image::CenterBottomFrameRender(HDC hdc, int x, int y, int frameX, int frameY)
{
	int changeX = x - mImageBuffer->frameWidth / 2;
	int changeY = y - mImageBuffer->frameHeight;

	FrameRender(hdc, changeX,changeY,frameX,frameY);
}


