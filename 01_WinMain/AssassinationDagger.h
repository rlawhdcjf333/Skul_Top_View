#pragma once
#include "Item.h"

class AssassinationDagger :public Item
{
	float mValue;
	float mSpeedValue;

public:

	AssassinationDagger(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};

