#pragma once
#include "GameObject.h"

class Item :public GameObject
{
protected:
	Image* mImage;

	wstring mItemName;
	wstring mExplanation;
	wstring mEffect;
	function <void(void)> mDeactivationFunc; //�������� ���� ��Ͽ��� �������� �� ������ �ڵ�

	float mDuration = 2;
	bool mIsCollision;

public:
	Item() {};
	virtual void Init();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Release();

	wstring GetImageKeyName() { return mImage->GetKeyName(); }
	wstring GetItemName() { return mItemName; }
	wstring GetEffect() { return mEffect; }
	wstring GetExplanation() { return mExplanation; }
	function <void(void)> GetDeactivationFunc() { return mDeactivationFunc; }
};