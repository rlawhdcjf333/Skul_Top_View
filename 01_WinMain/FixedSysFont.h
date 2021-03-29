#pragma once
#include "GameObject.h"
enum class FontColor : int {
	Blue,
	Red
};
class FixedSysFont : public GameObject
{
	int mStartX;
	int mStartY;
	wstring mNum;
	FontColor mColor;
	float mTime;
public :
	FixedSysFont(int x, int y, int sizeX, int sizeY, wstring num, FontColor color ,bool Camera = true);
	void Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Release() override;
	void FixedRender(HDC hdc);
	void FixedCamerRender(HDC hdc);
};

