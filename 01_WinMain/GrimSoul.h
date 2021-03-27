#pragma once
#include "Bullet.h"

class Animation;
class Enemy;
class GrimSoul :public Bullet {

	Image* mImage;
	Animation* mAnimation;
	Enemy* mTarget;

	float mMinDistance;
	float mDuration;

public:

	GrimSoul(Enemy* startUnit, float damage);
	void Update();
	void Release();
	void Render(HDC hdc);

};