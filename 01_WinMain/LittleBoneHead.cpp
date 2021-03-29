#include "pch.h"
#include "LittleBoneHead.h"

LittleBoneHead::LittleBoneHead(int indexX, int indexY)
{
	SetObjectOnTile(indexX, indexY);

	mIsActive = true;
	mIsDestroy = false;
	mName = "LittleBoneHead";

	mItemName = L"��Ʋ��";
	mEffect = L"���ռ� ������ ���� ���̷���. �ٸ� ���̷��濡 ���� ü���� �۾� ��Ʋ���̶� �Ҹ���.";
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
