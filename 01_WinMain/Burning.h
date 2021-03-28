#pragma once
#include "Condition.h"

class Burning :public Condition
{
	int mTick;

public:
	Burning(GameObject* target, int damage, float duration);
	void Update();
	void Render(HDC hdc);

};