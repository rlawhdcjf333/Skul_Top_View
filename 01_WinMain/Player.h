#pragma once
#include "GameObject.h"

class Player : public GameObject
{
	Image* mImage;

	float mTime;
	int mFrameX;

public:

	Player(float x, float y, float sizeX, float sizeY);
	void Init();
	void Update();
	void Release();
	void Render(HDC hdc);

};