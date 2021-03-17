#pragma once
#include "GameObject.h"

class MapObject:public GameObject
{
	Image* mImage;

public:
	MapObject(Image* image, int x, int y);
	void Init() {}
	void Update() {}
	void Release() {}
	void Render(HDC hdc);
	void AlphaRender(HDC hdc);

	wstring GetImageKey() const { return mImage->GetKeyName(); }
	void SetImage(Image* image) { mImage = image; }
};

