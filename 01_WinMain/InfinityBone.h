#pragma once
#include "Item.h"

class InfinityBone :public Item
{
	float mValue;

	bool mSkillCheck1;
	bool mSkillCheck2;

public:

	InfinityBone(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};




