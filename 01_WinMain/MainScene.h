#pragma once
#include "Scene.h"
class MainScene :public Scene
{
	Image* mBack;
	Image* mAnyButton;
	Animation* mAnm;
	float mCount;
public:
	void Init()override;
	void Update()override;
	void Render(HDC hdc)override;
	void Release() override;
};

