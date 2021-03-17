#pragma once

class Image;
class GameObject;
enum class TileType : int
{
	Normal = 1,
	Block = 2,
	Slow = 3,
	End = 4
};

class Tile
{
	Image* mImage;
	float mX;
	float mY;
	int mFrameX;
	int mFrameY;
	int mSizeX;
	int mSizeY;
	int mIndexX;
	int mIndexY;
	RECT mRect;
	Diam mDiam;
	TileType mTileType;
	bool mAttackTest = false;
	int mTestTime= 0;

	GameObject* mObject;
public:
	Tile(Image* pImage,  float x, float y, int frameX, int frameY, int sizeX, int sizeY, int indexX, int indexY);
	void Render(HDC hdc);
	void AlphaRender(HDC hdc);
	void SelectRender(HDC hdc);
	void SelectRenderBlue(HDC hdc);
	void SelectRenderMargenta(HDC hdc);
	
	Image* GetImage() { return mImage; }
	void SetImage(Image* pImage) { mImage = pImage; }
	
	RECT GetRect() { return mRect; }
	
	int GetFrameX() { return mFrameX;}
	void SetFrameX(int val) { mFrameX = val; }
	int GetFrameY() { return mFrameY; }
	void SetFrameY(int val) { mFrameY = val; }

	int GetIndexX() { return mIndexX; }
	int GetIndexY() { return mIndexY; }

	int GetCenterX() { return mX+TileSizeX/2; }
	int GetCenterY() { return mY+TileSizeY/2; }

	float GetX() { return mX; }
	float GetY() { return mY; }

	void SetObject(GameObject* object) { mObject = object;}
	void SetType(TileType val) { mTileType = val; }
	TileType GetType() { return mTileType; }

	void AttackDamage(int damage) {
		if (mObject != nullptr) {
			mObject->Damage(damage);
		}
		mAttackTest = true;
	}
};