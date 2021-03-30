#pragma once
#include "Item.h"

class ManaEater :public Item
{
	float mValue;

public:

	ManaEater(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};



