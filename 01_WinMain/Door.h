#pragma once
#include "GameObject.h"

class Door : public GameObject
{
	Image* mImage;
	Animation* mAnm;

public:
	Door(int x, int y);
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);
};

