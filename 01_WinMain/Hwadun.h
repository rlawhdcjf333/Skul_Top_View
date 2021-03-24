#pragma once
#include "Bullet.h"


class Animation;
class Hwadun :public Bullet {

	Image* mImage;
	Animation* mAnimation;

public:

	Hwadun(GameObject* startUnit, float damage, float angle);
	void Update();
	void Release();
	void Render(HDC hdc);

};