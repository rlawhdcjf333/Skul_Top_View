#pragma once
#include "GameObject.h"
enum class BulletType : int {
	Straight = 1,
	Trajectory,
	SkulHead,
	Piercing,
	Flask,
	FrameProjectile,
	Barricade,
	MeteorStrike
};

class Bullet : public GameObject
{
	Image* mImage;
	float mAngle;
	BulletType mType;
	float mSpeed;
	float mRange;
	int mCurrentFrameX;
	int mCurrentFrameY;
	float mFrameTick;

public:
	Bullet(Image* image, string name, GameObject* object, int damage, float speed, float range, float angle, BulletType type);
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);
	void Move();

	void Damage(int a);
	void Explosion(int damage, int range = 2);
	BulletType GetType() { return mType; }

};

