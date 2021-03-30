#include "pch.h"
#include "MagicalAtkBuff.h"

MagicalAtkBuff::MagicalAtkBuff(int percentage, float duration, string name)
{
	mName = name;

	if (mName == "HatredStone" and Obj->FindObject(ObjectLayer::Condition, "HatredStone"))
	{
		dynamic_cast<Condition*>(Obj->FindObject(ObjectLayer::Condition, "HatredStone"))->SetDuration(duration);
		delete(this);
		return;
	}
	if (mName == "RitualDaggerBuff" and Obj->FindObject(ObjectLayer::Condition, "RitualDaggerBuff"))
	{
		dynamic_cast<Condition*>(Obj->FindObject(ObjectLayer::Condition, "RitualDaggerBuff"))->SetDuration(duration);
		delete(this);
		return;
	}

	else
	{
		mIsActive = true;
		mIsDestroy = false;

		mDuration = duration;

		mValue = mMagicalAttackPower * (float)percentage / 100.f;

		SKUL->SetPhysicalAtk(mMagicalAttackPower + mValue);

		Obj->AddObject(ObjectLayer::Condition, this);
	}

}

void MagicalAtkBuff::Update()
{
	mDuration -= dTime;
	if (mDuration < 0) mIsDestroy = true;
}

void MagicalAtkBuff::Render(HDC hdc)
{
}

void MagicalAtkBuff::Release()
{
	SKUL->SetPhysicalAtk(mPhysicalAttackPower - mValue);
}