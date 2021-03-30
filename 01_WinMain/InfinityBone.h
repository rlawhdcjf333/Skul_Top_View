#pragma once
#include "Item.h"

class InfinityBone :public Item
{
	float mValue;

public:

	InfinityBone(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};




