#pragma once
#include "Item.h"

class HighWarlockHead :public Item
{



public:

	HighWarlockHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};
