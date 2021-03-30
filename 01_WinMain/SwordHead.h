#pragma once
#include "Item.h"

class SwordHead :public Item
{





public:

	SwordHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};
