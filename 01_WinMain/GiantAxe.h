#pragma once
#include "Item.h"

class GiantAxe :public Item
{
	float mValue;

public:

	GiantAxe(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};