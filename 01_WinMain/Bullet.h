#pragma once
#include "GameObject.h"
enum class BulletType : int {
	Straight = 1,
	FrameProjectile,
	Flask,
	Barricade,
	Protect,
	Mark
};

class Bullet : public GameObject
{
protected:
	Image* mImage;
	float mAngle;
	BulletType mType;
	float mSpeed;
	float mRange;
	int mCurrentFrameX;
	int mCurrentFrameY;
	float mFrameTick;

public:
	Bullet() {};
	Bullet(Image* image, string name, GameObject* object, int damage, float speed, float range, float angle, BulletType type);
	void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	
	void Move();
	void Damage(int a);
	void Explosion(int damage, int range = 2, function <void(void)> func =[]() {});
	BulletType GetType() { return mType; }

};

