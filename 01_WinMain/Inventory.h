#pragma once

class Item;
class Inventory
{
	Image* mImage;

	Item* mFirstSkul;
	Item* mSecondSkul;
	vector <Item*> mItemList;
	bool mToggleInventory;
	float mDelay;

	Item* mCurrentItem;
	
	//보유 스컬
	RECT mHoldingSkuls;
	RECT mFirstSkulSlot;
	RECT mSecondSkulSlot;
	//현재 스컬
	RECT mCurrent;
	RECT mCurrentSkulSlot;

	// 보유 아이템
	RECT mHoldingItems;
	RECT mItemSlot[9];
	//이미지 영역
	RECT mImageArea;
	//이름 영역
	RECT mItemNameArea;
	//배경 스토리 영역
	RECT mExplanationArea;
	//패시브, 효과, 교대 기술
	RECT mEffectArea;

	//각인1 스킬1
	RECT mDetailImageSlot1;
	RECT mDetailINameSlot1;
	RECT mDetailExplanationSlot1;

	//각인2 스킬2
	RECT mDetailImageSlot2;
	RECT mDetailINameSlot2;
	RECT mDetailExplanationSlot2;


public:

	Inventory();
	void Update();
	void Render(HDC hdc);
	void Release();

	void GetItem(Item* item) { mItemList.push_back(item);}
	void GetSkul(Item* item);

	bool GetIsToggle() { return mToggleInventory; }
	void SetIsToggle(bool b) { mToggleInventory = b; }

	Item* GetFirstSkul() { return mFirstSkul; }

};