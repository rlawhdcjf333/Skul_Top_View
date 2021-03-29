#pragma once
#include "Item.h"

class GrimReaperHead :public Item
{



public:

	GrimReaperHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};
