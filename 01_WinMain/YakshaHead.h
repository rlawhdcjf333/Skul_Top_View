#pragma once
#include "Item.h"

class YakshaHead :public Item
{



public:

	YakshaHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};
