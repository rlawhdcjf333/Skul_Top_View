#pragma once
#include "GameObject.h"

class Condition :public GameObject
{
protected:

	float mDuration;
	GameObject* mTarget;

public:

	Condition() {};
	void Init() {};
	void Release() {};
	void Update();
	void Render(HDC hdc);

};