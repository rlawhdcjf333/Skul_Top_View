#pragma once
#include "GameObject.h"
class MapObjectPallete : public GameObject
{

	Image* mImage;

public:

	MapObjectPallete(Image* image, int x, int y);
	void Init() {}
	void Update() {}
	void Release() {}
	void Render(HDC hdc);
	void Render(HDC hdc, LONG x, LONG y);

	wstring GetImageKey() const { return mImage->GetKeyName(); }
	Image* GetImage() { return mImage; }
	void SetImage(Image* image) { mImage = image; }
};

