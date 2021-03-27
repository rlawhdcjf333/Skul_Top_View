#pragma once
#include "Condition.h"

class Animation;
class TetanusBuff : public Condition
{
	Image* mImage;
	Animation* mAnimation;

public:

	TetanusBuff(GameObject* target, float duration);
	void Update();
	void Render(HDC hdc);
	void Release();

};