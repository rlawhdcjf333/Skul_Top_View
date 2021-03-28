#include "pch.h"
#include "PhysicalAtkBuff.h"

PhysicalAtkBuff::PhysicalAtkBuff(int percentage, float duration)
{
	mName = "PhysicalAtkBuff";
	mIsActive = true;
	mIsDestroy = false;

	mDuration = duration;

	mValue = mPhysicalAttackPower * percentage / 100;
	if (mValue == 0) mValue = 1;

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
