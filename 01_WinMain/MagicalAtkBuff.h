#pragma once
#include "Condition.h"

class MagicalAtkBuff : public Condition
{

	float mValue;

public:

	MagicalAtkBuff(int percentage, float duration, string name = "PhyscialAtkBuff");
	void Update();
	void Render(HDC hdc);
	void Release();

};