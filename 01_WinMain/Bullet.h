#pragma once
#include "GameObject.h"
enum class BulletType : int {
	Straight=1,
	Trajectory,
	SkulHead
};

class Bullet : public GameObject
{
	Image* mImage;
	float mAngle;
	BulletType mType;
	float mSpeed;
	float mRange;

public :
	Bullet(Image* image,string name, GameObject* object,int damage, float speed , float range , float angle, BulletType type);
	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);
	void Move();

	void Damage(int a);
};

