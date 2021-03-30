#pragma once
#include "Item.h"

class BerserkersGlove :public Item
{
	float mValue;
	bool mIsApplied;

public:

	BerserkersGlove(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};






