#pragma once

class Image;
class Tile
{
	Image* mImage;
	float mX;
	float mY;
	int mFrameX;
	int mFrameY;
	int mSizeX;
	int mSizeY;

	RECT mRect;
	Diam mDiam;



public:

	Tile(Image* pImage,  float x, float y, int frameX, int frameY, int sizeX, int sizeY);
	void Render(HDC hdc);
	void SelectRender(HDC hdc);

	Image* GetImage() { return mImage; }
	void SetImage(Image* pImage) { mImage = pImage; }
	
	RECT GetRect() { return mRect; }
	
	int GetFrameX() { return mFrameX;}
	void SetFrameX(int val) { mFrameX = val; }
	int GetFrameY() { return mFrameY; }
	void SetFrameY(int val) { mFrameY = val; }

	float GetX() { return mX; }
	float GetY() { return mY; }

	//HRGN GetRegion() 이거 작동이 너무 느림
	//{ 
	//	POINT points[4] = { mDiam.top, mDiam.right, mDiam.bottom, mDiam.left };
	//	return CreatePolygonRgn(points, 4, ALTERNATE);
	//}
};