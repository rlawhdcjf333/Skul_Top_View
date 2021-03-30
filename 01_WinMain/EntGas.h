#pragma once
#include "GameObject.h"
class EntGas :public GameObject
{
public :
	EntGas(int x, int y);
	void Init();
	void Update();
	void Render(HDC hdc);
	void Release();
};

