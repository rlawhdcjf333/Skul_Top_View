#pragma once
#include "Item.h"

class MedalOfCarleon :public Item
{
	float mValue;
	bool mButton;
public:

	MedalOfCarleon(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};

