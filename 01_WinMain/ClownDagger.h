#pragma once
#include "Bullet.h"

class ClownDagger : public Bullet {
	Image* mImage;
	int mFrameY;

	bool mIsExplosive;

public:

	ClownDagger(GameObject* obj, int damage, float angle, float range, bool isExplosive = false);
	void Update();
	void Render(HDC hdc);
	void Damage(int val);
};