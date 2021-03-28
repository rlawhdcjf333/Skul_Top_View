#pragma once
#include "Scene.h"

class Tile;
class Player;
class GameScene4 : public Scene
{
	vector <vector<Tile*>> mTileList;
public:
	int x;
	int y;
	int offsetX;
	int offsetY;
	class Image* mBack;
	void Init()override;
	void Update()override;
	void Render(HDC hdc)override;
	void Release() override;

	void MapLoad();

};