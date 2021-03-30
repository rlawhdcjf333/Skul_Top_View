#pragma once
#include "Item.h"

class WarriorHead :public Item
{



public:

	WarriorHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};
