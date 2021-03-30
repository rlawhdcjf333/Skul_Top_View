#pragma once
#include "Item.h"

class NinjaHead :public Item
{



public:

	NinjaHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};
