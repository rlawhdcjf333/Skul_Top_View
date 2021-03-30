#pragma once
#include "GameObject.h"
enum class AttackType : int {
	Side= 1,
	Stab,
	Whirlwind,
	RangedAttack,
	All
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

	RECT mHitBox;

	float mSpeed;
	float mInitSpeed;
	float mAngle;
	float mMoveAngle;

	float mInitDashCoolTime;
	float mDashCoolTime;
	bool mIsDash;
	int mDashCount;

	int mPathIndex = 0;

	float mSkill1CoolTime;
	float mSkill2CoolTime;


public:

	Player(int indexX, int indexY, float sizeX, float sizeY);
	void Init();
	void Update();
	void Release();
	void Render(HDC hdc);

	void Move(float speed);
	void Dash(int dist, bool isBack = false);
	void SetInitDashCoolTime(float val) { mInitDashCoolTime = val; }
	float GetInitDashCoolTime() { return mInitDashCoolTime; }
	float GetDashCoolTime() { return mDashCoolTime; }
	bool GetIsDash() { return mIsDash; }

	void Attack(int damage, int range, AttackType type, bool isBack = false);

	virtual void Skill1() {};
	virtual void Skill2() {};
	float const GetSkill1CoolTime() { return mSkill1CoolTime; };
	float const GetSkill2CoolTime() { return mSkill2CoolTime; };

	void UpdateAngle() 
	{ 
		mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
		mMoveAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
	}

	wstring GetKeyName() { return mImage->GetKeyName(); }

	virtual void SkulSwitch(int indexX, int indexY);
	virtual void SkulReset() {};

	virtual void SetAnimation(int listNum) {};
	virtual void SetAttackSpeed() {};
	void PathReset() { mPath.clear(); mPathIndex = 1;}
	RECT GetHitBox() { return mHitBox; }
};

#define M (int)Motion::
#define LEFT (mAngle>PI/2 and mAngle<=3*PI/2)
#define M_LEFT (mMoveAngle>PI/2 and mMoveAngle<=3*PI/2)
#define RIGHT (mAngle>3*PI/2 or mAngle<=PI/2)
#define M_RIGHT (mMoveAngle>3*PI/2 or mMoveAngle<=PI/2)