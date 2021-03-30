#include "pch.h"
#include "Inventory.h"
#include "Item.h"
#include "Items.h"

Inventory::Inventory()
{
	IMAGEMANAGER->LoadFromFile(L"Inventory", Resources(L"/item/Inventory.bmp"), 1280, 720, true);
	IMAGEMANAGER->LoadFromFile(L"Black", Resources(L"Black.bmp"), 1280, 720, true);
	mImage = IMAGEMANAGER->FindImage(L"Inventory");

	mToggleInventory = false;
	mCurrentItem = nullptr;

	mCurrent = RectMake(328, 264, 156, 38);
	mCurrentSkulSlot = RectMake(386, 312, 40, 40);
	
	mHoldingSkuls = RectMake(328, 147, 156, 36);
	mFirstSkulSlot = RectMake(345, 196, 38, 38);
	mSecondSkulSlot = RectMake(429, 196, 38, 38);

	mHoldingItems = RectMake(328, 380, 156, 38);
	mItemSlot[0] = RectMakeCenter(320, 450, 45, 45);
	mItemSlot[1] = RectMakeCenter(405, 450, 45, 45);
	mItemSlot[2] = RectMakeCenter(490, 450, 45, 45);

	mItemSlot[3] = RectMakeCenter(320, 515, 45, 45);
	mItemSlot[4] = RectMakeCenter(405, 515, 45, 45);
	mItemSlot[5] = RectMakeCenter(490, 515, 45, 45);

	mItemSlot[6] = RectMakeCenter(320, 580, 45, 45);
	mItemSlot[7] = RectMakeCenter(405, 580, 45, 45);
	mItemSlot[8] = RectMakeCenter(490, 580, 45, 45);

	mImageArea = RectMake(828, 72, 73, 73);
	mItemNameArea = RectMake(708, 175, 311, 34);
	mExplanationArea = RectMake(571, 213,580,40);
	mEffectArea = RectMake(594, 274, 540, 75);

	mDetailImageSlot1 = RectMake(686, 396, 40, 40);
	mDetailINameSlot1 = RectMake(586, 444, 239, 28);
	mDetailExplanationSlot1 = RectMake(570, 500, 272, 121);

	mDetailImageSlot2 = RectMake(1002, 396, 40, 40);
	mDetailINameSlot2 = RectMake(586+1002-686, 444, 239, 28);
	mDetailExplanationSlot2 = RectMake(570+1002-686, 500, 272, 121);
		
	mDelay = 2;
}

void Inventory::Update()
{
	for (int i=0; i<mItemList.size() ; i++)
	{
		if (PtInRect(&mItemSlot[i], nonC_mousePosition))
		{
			mCurrentItem = mItemList[i];
		}
	}
	if (PtInRect(&mFirstSkulSlot, nonC_mousePosition))
	{
		mCurrentItem = mFirstSkul;
	}
	if (PtInRect(&mSecondSkulSlot, nonC_mousePosition))
	{
		mCurrentItem = mSecondSkul;
	}


	if (INPUT->GetKey('F') and mCurrentItem)
	{
		mDelay -= dTime;
	}
	if (mDelay < 0) //아이템 제삭 트리거
	{
		mDelay = 2; 
		if (mCurrentItem->GetType() == ItemType::CommonItem)
		{
			mCurrentItem->GetDeactivationFunc()();
			mItemList.erase(find(mItemList.begin(), mItemList.end(), mCurrentItem));
			mCurrentItem->Release();
			SafeDelete(mCurrentItem);
			mCurrentItem = nullptr;
		}
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
		SetBkMode(hdc, TRANSPARENT);

		IMAGEMANAGER->FindImage(L"Black")->AlphaRender(hdc, 0, 0, 0.6f);
		mImage->Render(hdc, 0, 0);

		CallFont(hdc, 25, [&]() 
			{
				DrawText(hdc, L"보유 스컬", 5, &mHoldingSkuls, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
				DrawText(hdc, L"현재 스컬", 5, &mCurrent, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
				DrawText(hdc, L"보유 아이템", 6, &mHoldingItems, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			});
		IMAGEMANAGER->FindImage(SKUL->GetCurrentSkul()->GetKeyName().append(L"Head"))->ScaleRender(hdc, mCurrentSkulSlot.left, mCurrentSkulSlot.top, 40, 40);

		for (int i=0; i<mItemList.size(); i++) //아이템 9개 좌하단 렌더링
		{
			if(mItemList[i])
				IMAGEMANAGER->FindImage(mItemList[i]->GetImageKeyName())->ScaleRender(hdc, mItemSlot[i].left, mItemSlot[i].top, 45, 45); 
		}
		
		if (mFirstSkul)
			IMAGEMANAGER->FindImage(mFirstSkul->GetImageKeyName())->ScaleRender(hdc, mFirstSkulSlot.left, mFirstSkulSlot.top, 38, 38);
		if (mSecondSkul)
			IMAGEMANAGER->FindImage(mSecondSkul->GetImageKeyName())->ScaleRender(hdc, mSecondSkulSlot.left, mSecondSkulSlot.top, 38, 38);

		if (mCurrentItem) //현재 커서락된 아이템 우측 상세 렌더링
		{
			switch (mCurrentItem->GetType())
			{
			case ItemType::CommonItem:

				IMAGEMANAGER->FindImage(mCurrentItem->GetImageKeyName())->ScaleRender(hdc, mImageArea.left, mImageArea.top, 73, 73);
				IMAGEMANAGER->FindImage(mCurrentItem->GetSlot1Key())->ScaleRender(hdc, mDetailImageSlot1.left, mDetailImageSlot1.top, 40, 40);
				CallFont(hdc, 15, [&]() 
					{
						DrawText(hdc, mCurrentItem->GetItemName().c_str(), mCurrentItem->GetItemName().size(), &mItemNameArea, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
						DrawText(hdc, mCurrentItem->GetExplanation().c_str(), mCurrentItem->GetExplanation().size(), &mExplanationArea, DT_CENTER | DT_WORDBREAK |DT_VCENTER|DT_SINGLELINE);
						DrawText(hdc, mCurrentItem->GetEffect().c_str(), mCurrentItem->GetEffect().size(), &mEffectArea, DT_CENTER | DT_WORDBREAK);
						DrawText(hdc, mCurrentItem->GetSlot1Name().c_str(), mCurrentItem->GetSlot1Name().size(), &mDetailINameSlot1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
						DrawText(hdc, mCurrentItem->GetSlot1Explanation().c_str(), mCurrentItem->GetSlot1Explanation().size(), &mDetailExplanationSlot1, DT_CENTER |DT_WORDBREAK);
					}
				);
				break;

			case ItemType::SkulHead:

				IMAGEMANAGER->FindImage(mCurrentItem->GetImageKeyName())->ScaleRender(hdc, mImageArea.left, mImageArea.top, 73, 73);
				IMAGEMANAGER->FindImage(mCurrentItem->GetSlot1Key())->ScaleRender(hdc, mDetailImageSlot1.left, mDetailImageSlot1.top, 40, 40);
				IMAGEMANAGER->FindImage(mCurrentItem->GetSlot2Key())->ScaleRender(hdc, mDetailImageSlot2.left, mDetailImageSlot2.top, 40, 40);
				CallFont(hdc, 15, [&]()
					{
						DrawText(hdc, mCurrentItem->GetItemName().c_str(), mCurrentItem->GetItemName().size(), &mItemNameArea, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
						DrawText(hdc, mCurrentItem->GetExplanation().c_str(), mCurrentItem->GetExplanation().size(), &mExplanationArea, DT_CENTER | DT_WORDBREAK |DT_VCENTER|DT_SINGLELINE);
						DrawText(hdc, mCurrentItem->GetEffect().c_str(), mCurrentItem->GetEffect().size(), &mEffectArea, DT_CENTER | DT_WORDBREAK);

						DrawText(hdc, mCurrentItem->GetSlot1Name().c_str(), mCurrentItem->GetSlot1Name().size(), &mDetailINameSlot1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
						DrawText(hdc, mCurrentItem->GetSlot2Name().c_str(), mCurrentItem->GetSlot2Name().size(), &mDetailINameSlot2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

						DrawText(hdc, mCurrentItem->GetSlot1Explanation().c_str(), mCurrentItem->GetSlot1Explanation().size(), &mDetailExplanationSlot1, DT_CENTER | DT_WORDBREAK);
						DrawText(hdc, mCurrentItem->GetSlot2Explanation().c_str(), mCurrentItem->GetSlot2Explanation().size(), &mDetailExplanationSlot2, DT_CENTER | DT_WORDBREAK);
					}
				);
				break;
			}

		}

		SetBkMode(hdc, OPAQUE);
	}
}

void Inventory::Release()
{
	for (Item* elem : mItemList)
	{
		elem->Release();
		SafeDelete(elem);
	}
	if (mFirstSkul)
	{
		mFirstSkul->Release();
		SafeDelete(mFirstSkul)
	}
	if (mSecondSkul)
	{
		mSecondSkul->Release();
		SafeDelete(mSecondSkul)
	}
}

void Inventory::GetSkul(Item* item)
{
	if (mFirstSkul == nullptr)
	{
		mFirstSkul = item;
		mCurrentItem = mFirstSkul;
		return;
	}
	else if(mSecondSkul == nullptr)
	{
		mSecondSkul = item;
		mCurrentItem = mSecondSkul;
		return;
	}
	else if(mFirstSkul->GetImageKeyName() == SKUL->GetCurrentSkul()->GetKeyName().append(L"Head"))
	{
		mFirstSkul->SetObjectOnTile(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY());
		mFirstSkul->SetIsTrashed(true);
		mFirstSkul->SetDelay(0.5f);
		if (Obj->FindObject(ObjectLayer::Item, mFirstSkul->GetName()) == nullptr)
		{
			Obj->AddObject(ObjectLayer::Item, mFirstSkul);
		}
		mFirstSkul = item;
		mCurrentItem = mFirstSkul;
		return;
	}
	else if (mSecondSkul->GetImageKeyName() == SKUL->GetCurrentSkul()->GetKeyName().append(L"Head"))
	{
		mSecondSkul->SetObjectOnTile(SKUL->GetCurrentSkul()->GetIndexX(), SKUL->GetCurrentSkul()->GetIndexY());
		mSecondSkul->SetIsTrashed(true);
		mSecondSkul->SetDelay(0.5f);
		if (Obj->FindObject(ObjectLayer::Item, mFirstSkul->GetName()) == nullptr)
		{
			Obj->AddObject(ObjectLayer::Item, mSecondSkul);
		}
		mSecondSkul= item;
		mCurrentItem = mSecondSkul;
		return;
	}
	else
	{
		//do nothing
	}

}


