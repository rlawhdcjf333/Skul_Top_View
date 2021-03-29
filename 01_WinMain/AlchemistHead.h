#pragma once
#include "Item.h"

class AlchemistHead :public Item
{





public:

	AlchemistHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};
