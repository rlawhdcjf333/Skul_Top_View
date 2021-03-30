#pragma once
#include "Item.h"

class MinoHead :public Item
{



public:

	MinoHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};
