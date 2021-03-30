#pragma once
#include "Item.h"

class DeterminationOfKnight :public Item
{
	float mValue;
	bool mIsApplied;

public:

	DeterminationOfKnight(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};



