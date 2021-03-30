#pragma once
#include "Item.h"

class BerserkersGlove :public Item
{
	float mValue;

public:

	BerserkersGlove(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};






