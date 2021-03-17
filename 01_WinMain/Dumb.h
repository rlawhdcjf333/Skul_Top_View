#pragma once
#include "GameObject.h"

class Dumb : public GameObject {
	int mHp;
	bool mIsHit;
public :
	Dumb();
	void Init();
	void Update();
	void Render(HDC hdc);
	void Release() {}

	void Damage(int a);
};