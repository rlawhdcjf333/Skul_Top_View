#pragma once
#include "Item.h"

class HerosGlove :public Item
{
	float mValue;

public:

	HerosGlove(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};