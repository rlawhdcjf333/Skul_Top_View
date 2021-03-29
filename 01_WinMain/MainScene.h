#pragma once
#include "Scene.h"
class MainScene :public Scene
{
	Image* mBack;
public:
	void Init()override;
	void Update()override;
	void Render(HDC hdc)override;
	void Release() override;
};

