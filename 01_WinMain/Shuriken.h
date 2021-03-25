#pragma once
#include "Bullet.h"

class Shuriken :public Bullet
{
	Image* mImage;

public:

	Shuriken(GameObject* obj, int damage, float angle, float range);
	void Update();
	void Render(HDC hdc);


};