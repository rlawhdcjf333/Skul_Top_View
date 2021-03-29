#pragma once
#include "GameObject.h"

enum class EffectType : int
{
	Normal,
	Reverse,
	Follow
};

class Animation;
class Effect : public GameObject
{
	Image* mImage;
	Animation* mAnimation;
	wstring mNextImageKeyname;
	float mNextX;
	float mNextY;
	EffectType mType;
	float mAlpha;
	function<void(void)> mNextFunc;
public :
	Effect(wstring keyname,float x, float y, EffectType type);
	Effect(wstring keyname, float x, float y, function<void(void)> nextFunc);
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release()override;

	virtual void SetNextEffect(wstring keyname);
	virtual void SetNextEffect(wstring keyname, float x, float y);
	void SetUpdateTime(float val); 
	void Scaling(int x, int y, float alpha = 1.f) { mSizeX = x; mSizeY = y; mRect = RectMakeCenter(mX, mY, mSizeX, mSizeY); mAlpha = alpha; }
};

