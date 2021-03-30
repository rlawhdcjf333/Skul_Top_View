#pragma once
#include "Item.h"

class HunterHead :public Item
{



public:

	HunterHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};
