#pragma once
#include "Item.h"

class DimLightDarkcite :public Item
{
	float mValue;

public:

	DimLightDarkcite(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};

