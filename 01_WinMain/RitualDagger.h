#pragma once
#include "Item.h"

class RitualDagger :public Item
{
	float mValue;

public:

	RitualDagger(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};


