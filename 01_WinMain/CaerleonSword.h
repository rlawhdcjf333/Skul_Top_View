#pragma once
#include "Item.h"

class CaerleonSword :public Item
{
	float mValue;

public:

	CaerleonSword(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};