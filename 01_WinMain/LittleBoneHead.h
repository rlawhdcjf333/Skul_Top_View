#pragma once
#include "Item.h"

class LittleBoneHead :public Item
{

public:

	LittleBoneHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};