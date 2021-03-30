#pragma once
#include "Item.h"

class EntHead :public Item
{





public:

	EntHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};
