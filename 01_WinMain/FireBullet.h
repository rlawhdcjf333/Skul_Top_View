#pragma once
#include "Bullet.h"

class Enemy;
class FireBullet :public Bullet {

	Image* mImage;
	Enemy* mTarget;

	float mMinDistance;

public:

	FireBullet(Player* startUnit, float damage);
	void Update();
	void Release();
	void Render(HDC hdc);

};