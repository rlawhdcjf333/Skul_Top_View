#pragma once
#include "Scene.h"

class Tile;
class GameScene9 : public Scene
{
	vector <vector<Tile*>> mTileList;
	int x;
	int y;
	int offsetX;
	int offsetY;
	Image* mBack;
	Image* mWitch;
	Animation* mAnm;
	bool mDoorOpen = false;
	float mOpenTime = 1.0f;
	float mDoorEventTime = 3.0f;
public:
	void Init()override;
	void Update()override;
	void Render(HDC hdc)override;
	void Release() override;

	void MapLoad();

};