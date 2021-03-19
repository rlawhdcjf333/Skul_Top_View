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

	Player* mAlterSkul; //Ã¼ÀÎÂ¡ ½ºÄÃ

	float mSpeed;
	float mInitSpeed;
	float mAngle;

	float mInitDashCoolTime;
	float mDashCoolTime;
	bool mIsDash;
	int mDashCount;

	int mPathIndex = 0;

	int mAddHp;
	int mAddDamage;
public:

	Player(int indexX, int indexY, float sizeX, float sizeY);
	void Init();
	void Update();
	void Release();
	void Render(HDC hdc);

	void Move(float speed);
	void Dash(int dist);
	void SetInitDashCoolTime(float val) { mInitDashCoolTime = val; }
	float GetInitDashCoolTime() { return mInitDashCoolTime; }

	void Attack(int damage, int range, AttackType type);

	virtual void SkulSwitch(int indexX, int indexY);
	virtual void SkulReset() {};

	virtual void SetAnimation(int listNum) {};
};

#define M (int)Motion::
#define LEFT (mAngle>PI/2 and mAngle<=3*PI/2)
#define RIGHT (mAngle>3*PI/2 or mAngle<=PI/2)