#pragma once
#include "Item.h"

class CaerleonSword :public Item
{
	int mValue;

public:

	CaerleonSword(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};