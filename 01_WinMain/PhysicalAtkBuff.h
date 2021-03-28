#pragma once
#include "Condition.h"

class PhysicalAtkBuff : public Condition
{

	int mValue;

public:

	PhysicalAtkBuff(int percentage, float duration);
	void Update();
	void Render(HDC hdc);
	void Release();

};