#pragma once
#include "Item.h"

class BoneOfMana :public Item
{
	float mValue;

public:

	BoneOfMana(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();

};

