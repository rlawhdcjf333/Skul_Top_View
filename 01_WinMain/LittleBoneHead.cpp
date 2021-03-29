#include "pch.h"
#include "LittleBoneHead.h"

LittleBoneHead::LittleBoneHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "LittleBoneHead";

	mItemName = L"리틀본";
	mEffect = L"마왕성 경비대의 막내 스켈레톤. 다른 스켈레톤에 비해 체구가 작아 리틀본이라 불린다.";
	mExplanation = L"";

	IMAGEMANAGER->LoadFromFile(L"LittleBoneHead", Resources(L"item/LittleBoneHead.bmp"), 22, 22, true);
	mImage = IMAGEMANAGER->FindImage(L"LittleBoneHead");

	mSizeX = mImage->GetWidth();
	mSizeY = mImage->GetHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

	mIsCollision = false;

	mIsTrashed = true;
	Obj->AddObject(ObjectLayer::Item, this);
}

void LittleBoneHead::Update()
{
}

void LittleBoneHead::Render(HDC hdc)
{
}

void LittleBoneHead::Release()
{
}
