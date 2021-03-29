#pragma once
#include "Scene.h"

class Tile;
class Player;
class GameScene3 : public Scene
{
	vector <vector<Tile*>> mTileList;
	bool mDoorOpen = false;
	float mOpenTime = 1.0f;
	float mDoorEventTime = 3.0f;
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