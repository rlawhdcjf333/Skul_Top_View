#pragma once
#include "GameObject.h"

class Door : public GameObject
{
	Image* mImage;
	Animation* mAnm1;
	Animation* mAnm2;
	Animation* mCurrentAnm;
	bool mDoorOpen;
public:
	Door(int x, int y);
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	bool DoorOpenCheck() const { return mDoorOpen; }
};

