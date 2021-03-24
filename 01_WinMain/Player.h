#pragma once
#include "GameObject.h"
enum class AttackType : int {
	Side= 1,
	Stab,
	Whirlwind,
	RangedAttack,
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

	float mInitDashCoolTime;
	float mDashCoolTime;
	bool mIsDash;
	int mDashCount;

	int mPathIndex = 0;

	int mPhysicalAttackPower;
	int mMagicalAttackPower;
	float mAttackSpeed;

	bool mInvincibility;
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

	void Attack(int damage, int range, AttackType type, bool isBack = false);

	virtual void Skill1() {};
	virtual void Skill2() {};

	int const GetPhysicalAttackPower() { return mPhysicalAttackPower; }
	void SetPhysicalAttackPower(int val) { mPhysicalAttackPower = val; }

	int const GetMagicalAttackPower() { return mMagicalAttackPower; }
	void PhysicalAttackBuff(int percentage, float buffDuration);
	void AttackSpeedBuff(int percentage, float buffDuration);
	void AttackSpeedSet(float val) { mAttackSpeed = val; }
	void UpdateAngle() { mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY()); }

	virtual void SkulSwitch(int indexX, int indexY);
	virtual void SkulReset() {};

	virtual void SetAnimation(int listNum) {};
	virtual void SetAttackSpeed() {};
	
};

#define M (int)Motion::
#define LEFT (mAngle>PI/2 and mAngle<=3*PI/2)
#define RIGHT (mAngle>3*PI/2 or mAngle<=PI/2)