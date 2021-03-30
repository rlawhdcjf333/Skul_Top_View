#pragma once
#include "GameObject.h"
class BossFireBall : public GameObject
{
	Image* mImage;
	Animation* mAnimation;
	float mSpeed;
	float mAngle;
	float mMove;
public :
	BossFireBall(float x, float y, float angle, float speed);
	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();
};

