#pragma once
#include "Bullet.h"

class ClownBox : public Bullet
{
	Image* mImage;
	float mShowTime;

	float mInitThrow;
	float mDestinationY;

public:

	ClownBox(GameObject* obj, int damage, float angle);
	void Update();
	void Render(HDC hdc);

};