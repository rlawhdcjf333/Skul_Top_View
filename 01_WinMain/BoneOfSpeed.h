#pragma once
#include "Item.h"

class BoneOfSpeed :public Item
{
	float mValue;

public:

	BoneOfSpeed(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};



