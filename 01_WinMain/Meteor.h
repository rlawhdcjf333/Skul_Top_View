#pragma once
#include "Bullet.h"

class Animation;
class Meteor :public Bullet {

	Image* mImage;
	Animation* mAnimation;
	bool mIsCompleted;

public:

	Meteor(GameObject* startUnit, float speed, float angle, float damage, bool isCompleted);
	void Update();
	void Release();
	void Render(HDC hdc);

};