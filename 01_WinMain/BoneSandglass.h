#pragma once
#include "Item.h"

class BoneSandglass :public Item
{
	float mValue;

public:

	BoneSandglass(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};




