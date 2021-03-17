#include "pch.h"
#include "MapObject.h"

MapObject::MapObject(Image* image, int x, int y)
{
	mImage = image;
	mX = x;
	mY = y;	
	
	mRect = RectMakeBottom(mX,mY,image->GetWidth(),image->GetHeight());
}

void MapObject::Render(HDC hdc)
{
	CAMERA->Render(hdc, mImage, mRect.left, mRect.top);
}
void MapObject::AlphaRender(HDC hdc)
{
	CAMERA->AlphaRender(hdc, mImage, mRect.left, mRect.top,0.5f);
}

