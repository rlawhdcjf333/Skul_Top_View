#pragma once
#include "Item.h"

class LuckyStone :public Item
{
	float mValue;

public:

	LuckyStone(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};



