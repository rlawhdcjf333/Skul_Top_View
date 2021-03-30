#pragma once
#include "GameObject.h"
class FirePillar : public GameObject
{
	Image* mImage;
	class Animation* mAnimtion;
	bool mDamageCheck;
public :
	FirePillar(int x, int y,RECT rc);
	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();
};

