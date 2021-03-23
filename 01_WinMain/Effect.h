#pragma once
#include "GameObject.h"
class Effect : public GameObject
{
	Image* mImage;
	class Animation* mAnimation;
	wstring mNextImageKeyname;
	float mNextX;
	float mNextY;
public :
	Effect(wstring keyname,float x, float y);
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release()override;

	virtual void SetNextEffect(wstring keyname);
	virtual void SetNextEffect(wstring keyname, float x, float y);
};

