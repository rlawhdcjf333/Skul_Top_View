#pragma once
#include "Item.h"

class PettyThiefHead :public Item
{





public:

	PettyThiefHead(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};
