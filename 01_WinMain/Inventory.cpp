#include "pch.h"
#include "Inventory.h"
#include "Item.h"

Inventory::Inventory()
{
	IMAGEMANAGER->LoadFromFile(L"Inventory", Resources(L"/item/Inventory.bmp"), 1280, 720, true);
	IMAGEMANAGER->LoadFromFile(L"Black", Resources(L"Black.bmp"), 1280, 720, true);
	mImage = IMAGEMANAGER->FindImage(L"Inventory");

	mToggleInventory = false;
	mCurrentItem = nullptr;

	mItemSlot[0] = RectMakeCenter(320, 450, 45, 45);
	mItemSlot[1] = RectMakeCenter(405, 450, 45, 45);
	mItemSlot[2] = RectMakeCenter(490, 450, 45, 45);

	mItemSlot[3] = RectMakeCenter(320, 515, 45, 45);
	mItemSlot[4] = RectMakeCenter(405, 515, 45, 45);
	mItemSlot[5] = RectMakeCenter(490, 515, 45, 45);

	mItemSlot[6] = RectMakeCenter(320, 580, 45, 45);
	mItemSlot[7] = RectMakeCenter(405, 580, 45, 45);
	mItemSlot[8] = RectMakeCenter(490, 580, 45, 45);

	mItemNameArea = RectMake(708, 175, 311, 34);
	mEffectArea = RectMake(594, 274, 540, 75);
	mExplanationArea = RectMake(588, 423, 552,176);
		
	mDelay = 2;
}

void Inventory::Update()
{
	mCurrentItem = nullptr;
	for (int i=0; i<mItemList.size() ; i++)
	{
		if (PtInRect(&mItemSlot[i], nonC_mousePosition))
		{
			mCurrentItem = mItemList[i];
		}
	}

	if (INPUT->GetKey('F') and mCurrentItem)
	{
		mDelay -= dTime;
	}
	if (mDelay < 0) //아이템 제삭 트리거
	{
		mDelay = 2; 
		mCurrentItem->GetDeactivationFunc()();
		mItemList.erase(find(mItemList.begin(), mItemList.end(), mCurrentItem));
		mCurrentItem->Release();
		SafeDelete(mCurrentItem);
		mCurrentItem = nullptr;
	}
	if (INPUT->GetKeyUp('F'))
	{
		mDelay = 2;
	}
}

void Inventory::Render(HDC hdc)
{
	if (mToggleInventory)
	{
		IMAGEMANAGER->FindImage(L"Black")->AlphaRender(hdc, 0, 0, 0.6f);
		mImage->Render(hdc, 0, 0);
		for (int i=0; i<mItemList.size(); i++)
		{
			IMAGEMANAGER->FindImage(mItemList[i]->GetImageKeyName())->ScaleRender(hdc, mItemSlot[i].left, mItemSlot[i].top, 45, 45); 
		}

		if (mCurrentItem)
		{
			switch (mCurrentItem->GetType())
			{
			case ItemType::CommonItem:
				SetBkMode(hdc, TRANSPARENT);
				DrawText(hdc, mCurrentItem->GetItemName().c_str(), mCurrentItem->GetItemName().size(), &mItemNameArea, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
				DrawText(hdc, mCurrentItem->GetEffect().c_str(), mCurrentItem->GetEffect().size(), &mEffectArea, DT_CENTER | DT_WORDBREAK);
				DrawText(hdc, mCurrentItem->GetExplanation().c_str(), mCurrentItem->GetExplanation().size(), &mExplanationArea, DT_CENTER | DT_WORDBREAK);
				SetBkMode(hdc, OPAQUE);
				break;

			case ItemType::SkulHead:





				break;
			}
			

		}
	}
}

void Inventory::Release()
{
	for (Item* elem : mItemList)
	{
		elem->Release();
		SafeDelete(elem);
	}
}


