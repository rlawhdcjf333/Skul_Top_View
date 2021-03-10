#pragma once
#include "Scene.h"

class Tile;
class Player;
class GameScene : public Scene
{
	vector <vector<Tile*>> mTileList;
	Player* mPlayer;

public:

	void Init()override;
	void Update()override;
	void Render(HDC hdc)override;
	void Release() override;

	void MapLoad();

};