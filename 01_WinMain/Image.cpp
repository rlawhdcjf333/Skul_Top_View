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
	//�̹� �̹��� ���۰� �ִٸ� ����
	if (mImageBuffer != nullptr)
	{
		ReleaseBuffer();
	}

	HDC hdc = GetDC(_hWnd);

	mImageBuffer = new ImageBuffer();
	mImageBuffer->loadType = LoadType::Empty;
	mImageBuffer->registerID = 0;
	mImageBuffer->hdc = CreateCompatibleDC(hdc);	//�� HDC����
	mImageBuffer->bitmap = (HBITMAP)CreateCompatibleBitmap(hdc, width, height); //�� ��Ʈ�� ����
	mImageBuffer->oldBitmap = (HBITMAP)SelectObject(mImageBuffer->hdc, mImageBuffer->bitmap);	//���� ��Ʈ�� ����
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
hdc : �׸� HDC
x : �׸� ��ǥX
y : �׸� ��ǥY
*/
void Image::Render(HDC hdc, int x, int y)
{
	if (mIsTrans)
	{
		GdiTransparentBlt(
			hdc,					//�׸� ����(HDC)
			x,						//�׸� ��ǥX
			y,						//�׸� ��ǥY
			mImageBuffer->width,	//�׸� ���α���
			mImageBuffer->height,	//�׸� ���α���
			mImageBuffer->hdc,		//�׸� HDC
			0,						//�����ؿ� ���� ������ǥX
			0,						//�����ؿ� ���� ������ǥY
			mImageBuffer->width,	//�����ؿ� ���� ���α���
			mImageBuffer->height,	//�����ؿ� ���� ���α���
			mTransColor				//Transó���� ����
		);
	}
	else
	{
		//HDC�� ���۸� �ٸ� HDC���ۿ� ��� �������ִ� �Լ�
		BitBlt(hdc, x, y, mImageBuffer->width, mImageBuffer->height, mImageBuffer->hdc, 0, 0, SRCCOPY);
	}
}

/*
## Render ##
hdc : �׸� HDC
x : �׸� ��ǥ
y : �׸� ��ǥ
tempX : ����(�ؽ���)�κ��� �׸� �����ȼ�
tempY : ����(�ؽ���)�κ��� �׸� �����ȼ�
tempWidth : ������ tempX�κ��� �׸� �ʺ�
tempHeight : ������ tempY�κ��� �׸� ����
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
x : �׸� ��ǥ
y : �׸� ��ǥ
frameX : ����� ���� ������ �ε���
frameY : ����� ���� ������ �ε��� 
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


