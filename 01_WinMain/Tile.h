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
	int mZaxis;
	RECT mRect;
	Diam mDiam;
	TileType mTileType;
	bool mAttackTest = false;
	int mTestTime= 0;

	bool mIsTileEmpty; // 오브젝트가 비여 있으면 true;
	GameObject* mObject;
	vector<GameObject*> mObjects; //오브젝트를 담는 곳
public:
	Tile(Image* pImage,  float x, float y, int frameX, int frameY, int sizeX, int sizeY, int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void AlphaRender(HDC hdc);
	void SelectRender(HDC hdc);
	void SelectRenderBlue(HDC hdc);
	void SelectRenderMargenta(HDC hdc);

	int GetZaxis() { return mZaxis; }
	void SetZaxis(int zaxis) { mZaxis = zaxis; }
	
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

	//void SetObject(GameObject* object) { mObject = object;}
	void SetObject(GameObject* object) { mObjects.push_back(object); }
	void ClearObject(GameObject* object) { mObjects.erase(find(mObjects.begin(), mObjects.end(), object)); }
	bool GetTileEmpty() const { return mIsTileEmpty; }
	void SetType(TileType val) { mTileType = val; }
	TileType GetType() { return mTileType; }

	void AttackDamage(int damage);
	void EnemyAttack(int damage);
};