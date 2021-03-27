#pragma once
#include "Bullet.h"

class LittleHead :public Bullet
{
	Image* mImage;
	float mDestinationY;
	float mInitThrow;

public:

	LittleHead(GameObject* obj, int damage, float angle, float range);
	void Update();
	void Render(HDC hdc);
	void Damage(int val);

};