#pragma once
#include "GameObject.h"

enum class ItemType :int {

	SkulHead=0,
	CommonItem,
};
class Item :public GameObject
{
protected:
	//������ Ȥ�� ������ �̹���
	Image* mImage; 

	//������ Ȥ�� ���� �̸�
	wstring mItemName; 
	//������ Ȥ�� ������ ��� �̾߱�
	wstring mExplanation; 
	//������ ȿ�� ������ ������ �нú�ȿ��+���� ��� ����
	wstring mEffect; 

	//������ ���� 1 Ȥ�� ���� ��ų 1 �̹���
	Image* mSlot1Image; 
	//������ ���� 2 Ȥ�� ���� ��ų 2 �̹���
	Image* mSlot2Image; 
	//������ ���� 1 �̸� Ȥ�� ���� ��ų 1 �̸�
	wstring mSlot1Name; 
	//������ ���� 2 �̸� Ȥ�� ���� ��ų 2 �̸�
	wstring mSlot2Name;
	//������ ���� 1 ���� Ȥ�� ���� ��ų 1 ����+��ٿ�
	wstring mSlot1Explanation;
	//������ ���� 2 �̸� Ȥ�� ���� ��ų 2 �̸�
	wstring mSlot2Explanation;

	function <void(void)> mActivationFunc; //Ư���� ��Ƽ�� ȿ���� �ִ� ��� �������� ������ �ڵ�
	function <void(void)> mDeactivationFunc; //�������� ���� ��Ͽ��� �������� �� ������ �ڵ� 

	ItemType mType;
	float mDuration = 2;
	float mDelay = 0;
	bool mIsCollision;

public:
	Item() {};
	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();
	void SetDelay(float val) { mDelay = val; }
	void Activation() { mActivationFunc(); }

	wstring GetImageKeyName() { return mImage->GetKeyName(); }
	wstring GetItemName() { return mItemName; }
	wstring GetEffect() { return mEffect; }
	wstring GetExplanation() { return mExplanation; }

	wstring GetSlot1Key() { return mSlot1Image->GetKeyName(); }
	wstring GetSlot2Key() { return mSlot2Image->GetKeyName(); }
	wstring GetSlot1Name() { return mSlot1Name; }
	wstring GetSlot2Name() {return mSlot2Name;}
	wstring GetSlot1Explanation() { return mSlot1Explanation; }
	wstring GetSlot2Explanation() { return mSlot2Explanation; }

	ItemType GetType() { return mType; }
	function <void(void)> GetDeactivationFunc() { return mDeactivationFunc; }

};