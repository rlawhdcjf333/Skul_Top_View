#pragma once
#include "Condition.h"

class PhysicalAtkBuff : public Condition
{

	float mValue;

public:

	PhysicalAtkBuff(int percentage, float duration, string name ="PhyscialAtkBuff");
	void Update();
	void Render(HDC hdc);
	void Release();

};