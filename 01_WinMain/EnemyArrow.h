#pragma once
#include "GameObject.h"
class EnemyArrow : public GameObject
{
	Image* mImage;
	float mAngle;
	float mSpeed;
	float mMove;
public :
	EnemyArrow(float x, float y, float angle, float speed);
	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();
};

