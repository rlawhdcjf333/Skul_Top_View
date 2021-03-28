#pragma once
#include "Bullet.h"


class Animation;
class AlchPassive :public Bullet {

	Image* mImage;
	Animation* mAnimation;

public:

	AlchPassive(GameObject* startUnit, float damage, float angle);
	void Update();
	void Release();
	void Render(HDC hdc);

};