#pragma once
#include "GameObject.h"
class LoopEffect : public GameObject
{
	Image* mImage;
	float mAlpha;
public:
	LoopEffect(wstring keyname, float x, float y);
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release()override;

	void Scaling(int x, int y, float alpha = 1.f) { mSizeX = x; mSizeY = y; mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY); mAlpha = alpha; }
};

