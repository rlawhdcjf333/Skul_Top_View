#pragma once
#include "Bullet.h"

class Animation;
class WarlockOrb :public Bullet {

	Image* mImage;
	Animation* mAnimation;
	bool mIsCompleted;

public:

	WarlockOrb(GameObject* startUnit, float speed, float angle, float range, float damage, bool isCompleted);
	void Update();
	void Release();
	void Render(HDC hdc);

};