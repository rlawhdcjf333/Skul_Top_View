#pragma once
#include "Item.h"

class RitualStaff :public Item
{
	float mValue;

public:

	RitualStaff(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};




