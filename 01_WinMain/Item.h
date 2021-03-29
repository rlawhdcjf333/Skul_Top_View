#pragma once
#include "GameObject.h"

enum class ItemType :int {

	SkulHead=0,
	CommonItem,
};
class Item :public GameObject
{
protected:
	//아이템 혹은 스컬의 이미지
	Image* mImage; 

	//아이템 혹은 스컬 이름
	wstring mItemName; 
	//아이템 혹은 스컬의 배경 이야기
	wstring mExplanation; 
	//아이템 효과 내지는 스컬의 패시브효과+교대 기술 설명
	wstring mEffect; 

	//아이템 각인 1 혹은 스컬 스킬 1 이미지
	Image* mSlot1Image; 
	//아이템 각인 2 혹은 스컬 스킬 2 이미지
	Image* mSlot2Image; 
	//아이템 각인 1 이름 혹은 스컬 스킬 1 이름
	wstring mSlot1Name; 
	//아이템 각인 2 이름 혹은 스컬 스킬 2 이름
	wstring mSlot2Name;
	//아이템 각인 1 설명 혹은 스컬 스킬 1 설명+쿨다운
	wstring mSlot1Explanation;
	//아이템 각인 2 이름 혹은 스컬 스킬 2 이름
	wstring mSlot2Explanation;

	function <void(void)> mActivationFunc; //특별히 액티브 효과가 있는 경우 매프레임 실행할 코드
	function <void(void)> mDeactivationFunc; //아이템을 소유 목록에서 제거했을 때 실행할 코드 

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