#include "pch.h"
#include "PhysicalAtkBuff.h"

PhysicalAtkBuff::PhysicalAtkBuff(int percentage, float duration, string name)
{
	mName = name;

	if (mName == "BerserkerPhysicalAtkBuff" and Obj->FindObject(ObjectLayer::Condition, "BerserkerPhysicalAtkBuff"))
	{
		Obj->ReleaseObject(ObjectLayer::Condition, "BerserkerPhysicalAtkBuff");
	}
	
	mIsActive = true;
	mIsDestroy = false;

	mDuration = duration;

	mValue = mPhysicalAttackPower * (float)percentage / 100.f;

	SKUL->SetPhysicalAtk(mPhysicalAttackPower + mValue);

	Obj->AddObject(ObjectLayer::Condition, this);
	
}

void PhysicalAtkBuff::Update()
{
	mDuration -= dTime;
	if (mDuration < 0) mIsDestroy = true;
}

void PhysicalAtkBuff::Render(HDC hdc)
{
}

void PhysicalAtkBuff::Release()
{
	SKUL->SetPhysicalAtk(mPhysicalAttackPower - mValue);
}