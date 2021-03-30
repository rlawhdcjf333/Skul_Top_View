#pragma once
#include "Item.h"

class RageClaw :public Item
{
	float mValue;

public:

	RageClaw(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};








