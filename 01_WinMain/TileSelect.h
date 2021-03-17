#pragma once
#include "GameObject.h"
#include "Tile.h"

class Tile;
class TileSelect :public GameObject
{
	Tile* mCurrentTile;
	
public:
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);


	int GetIndexX() { if (mCurrentTile) { return  mCurrentTile->GetIndexX(); } }
	int GetIndexY() { if (mCurrentTile) { return mCurrentTile->GetIndexY(); } }
	int GetTileX() { if (mCurrentTile) { return mCurrentTile->GetX(); } }
	int GetTileY() { if (mCurrentTile) { return mCurrentTile->GetY(); } }

};