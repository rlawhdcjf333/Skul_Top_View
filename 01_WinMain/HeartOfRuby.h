#pragma once
#include "Item.h"

class HeartOfRuby :public Item
{
	float mValue;

public:

	HeartOfRuby(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};







