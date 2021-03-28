#pragma once

class Item;
class Inventory
{
	Image* mImage;

	struct AppliedItem {

		wstring mImageKeyName;
		wstring mItemName;
		wstring mExplanation;
		wstring mEffect;
		function<void(void)> mDeactivationFunc;
	};

	vector <Item*> mItemList;
	bool mToggleInventory;
	float mDelay;

	Item* mCurrentItem;
	RECT mItemSlot[9];
	RECT mItemNameArea;
	RECT mExplanationArea;
	RECT mEffectArea;

public:

	Inventory();
	void Update();
	void Render(HDC hdc);
	void Release();

	void GetItem(Item* item) { mItemList.push_back(item);}
	bool GetIsToggle() { return mToggleInventory; }
	void SetIsToggle(bool b) { mToggleInventory = b; }


};