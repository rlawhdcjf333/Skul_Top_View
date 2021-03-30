#pragma once
#include "Item.h"

class ClownHead :public Item
{





public:

	ClownHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};
