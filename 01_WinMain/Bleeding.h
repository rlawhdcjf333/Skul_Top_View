#pragma once
#include "Condition.h"

class Bleeding : public Condition
{

	int mTick;

public:

	Bleeding(GameObject* target, int damage, float duration);
	void Update();
	void Render(HDC hdc);


};