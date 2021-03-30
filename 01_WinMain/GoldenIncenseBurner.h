#pragma once
#include "Item.h"

class GoldenIncenseBurner :public Item
{
	float mValue;

public:

	GoldenIncenseBurner(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};




