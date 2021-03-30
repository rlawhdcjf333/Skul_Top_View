#pragma once
#include "Item.h"

class HeartOfRuby :public Item
{
	float mValue;
	bool mIsApplied;

public:

	HeartOfRuby(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};







