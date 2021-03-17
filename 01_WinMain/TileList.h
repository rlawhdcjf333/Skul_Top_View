#pragma once
class Tile;
class TileList
{
	Singleton(TileList)

private:
	typedef vector<vector<Tile*>> TileMap;
	TileMap mTileList;
	
public:
	inline void SetMap(TileMap tileMap) { mTileList = tileMap; }
	inline TileMap GetMap() { return mTileList; }
	bool MapAreaCheck(float a, float b); //甘 康开 眉农
	bool MapAreaCheck(LONG a, LONG b); //甘 康开 眉农
};

#define TILE  TileList::GetInstance()->GetMap()
#define TILELIST TileList::GetInstance()
#define TILESizeX  TileList::GetInstance()->GetMap()[0].size()
#define TILESizeY  TileList::GetInstance()->GetMap().size()
#define TILECHECK(a,b) TileList::GetInstance()->MapAreaCheck(a,b)