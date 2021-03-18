#pragma once
#include "GameObject.h"
enum class AttackType : int {
	Side= 1,
	Stab,
	Whirlwind,
	RangedAttack
};

class Animation;
class TileSelect;
class Tile;
class Player : public GameObject
{
protected:
	Image* mImage;
	TileSelect* mTileSelect;
	vector<Tile*> mPath;
	
	float mSpeed;
	float mInitSpeed;
	float mAngle;
	float mTime;
	int mFrameX;

	bool mIsDash;
	int mPathIndex = 0;

public:

	Player(int indexX, int indexY, float sizeX, float sizeY);
	void Init();
	void Update();
	void Release();
	void Render(HDC hdc);

	void Move(float speed);
	void Dash(int dist);
	void Attack(int damage, int range, AttackType type);
};

#define M (int)Motion::
#define LEFT (mAngle>PI/2 and mAngle<=3*PI/2)
#define RIGHT (mAngle>3*PI/2 or mAngle<=PI/2)