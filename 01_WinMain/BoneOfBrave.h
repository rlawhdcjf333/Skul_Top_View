#pragma once
#include "Item.h"

class BoneOfBrave :public Item
{
	float mValue;
	bool mIsEffective;
	float mDelay;

public:

	BoneOfBrave(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};