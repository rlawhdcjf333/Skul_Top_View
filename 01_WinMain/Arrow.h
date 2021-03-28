#pragma once
#include "Bullet.h"

class Arrow :public Bullet
{
	Image* mImage;
	bool mIsPiercing;
	vector <GameObject*> mTargets;

public:

	Arrow(GameObject* startUnit, int damage, float angle, bool isPiercing);
	void Update();
	void Render(HDC hdc);
	void Release();

};