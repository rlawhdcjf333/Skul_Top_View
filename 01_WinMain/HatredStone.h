#pragma once
#include "Item.h"

class HatredStone :public Item
{
	float mValue;

public:

	HatredStone(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};


