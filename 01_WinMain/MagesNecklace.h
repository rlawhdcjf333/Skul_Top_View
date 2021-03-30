#pragma once
#include "Item.h"

class MagesNecklace :public Item
{
	float mValue;

public:

	MagesNecklace(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};

