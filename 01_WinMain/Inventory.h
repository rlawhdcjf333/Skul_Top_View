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
	
	//���� ����
	RECT mHoldingSkuls;
	RECT mFirstSkulSlot;
	RECT mSecondSkulSlot;
	//���� ����
	RECT mCurrent;
	RECT mCurrentSkulSlot;

	// ���� ������
	RECT mHoldingItems;
	RECT mItemSlot[9];
	//�̹��� ����
	RECT mImageArea;
	//�̸� ����
	RECT mItemNameArea;
	//��� ���丮 ����
	RECT mExplanationArea;
	//�нú�, ȿ��, ���� ���
	RECT mEffectArea;

	//����1 ��ų1
	RECT mDetailImageSlot1;
	RECT mDetailINameSlot1;
	RECT mDetailExplanationSlot1;

	//����2 ��ų2
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