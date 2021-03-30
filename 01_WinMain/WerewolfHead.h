#pragma once
#include "Item.h"

class WerewolfHead :public Item
{





public:

	WerewolfHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};
