#pragma once
#include "GameObject.h"

enum class EffectType : int
{
	Normal,
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
public :
	Effect(wstring keyname,float x, float y, EffectType type);
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
	virtual void Release()override;

	virtual void SetNextEffect(wstring keyname);
	virtual void SetNextEffect(wstring keyname, float x, float y);
	void SetUpdateTime(float val); 
};

