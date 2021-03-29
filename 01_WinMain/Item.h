#pragma once
#include "GameObject.h"

enum class ItemType :int {

	SkulHead=0,
	CommonItem,
};
class Item :public GameObject
{
protected:
	Image* mImage;

	wstring mItemName;
	wstring mExplanation;
	wstring mEffect;
	function <void(void)> mDeactivationFunc; //아이템을 소유 목록에서 제거했을 때 실행할 코드

	ItemType mType;
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

	ItemType GetType() { return mType; }
	function <void(void)> GetDeactivationFunc() { return mDeactivationFunc; }
};