#pragma once
#include "Item.h"

class BerserkerHead :public Item
{



public:

	BerserkerHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};
