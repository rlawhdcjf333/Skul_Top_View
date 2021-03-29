#pragma once
#include "Condition.h"

class AtkSpeedBuff : public Condition
{

	float mValue;

public:

	AtkSpeedBuff(int percentage, float duration, string Name = "AtkSpeedBuff");
	void Update();
	void Render(HDC hdc);
	void Release();

};