#include "pch.h"
#include "Burning.h"
#include "Effect.h"

Burning::Burning(GameObject* target, int damage, float duration)
{
	mName = "Burning";
	mIsActive = true;
	mIsDestroy = false;

	mTarget = target;
	mDamage = damage;
	mDuration = duration;

	mTick = (int)duration;

	Obj->AddObject(ObjectLayer::Condition, this);
}

void Burning::Update()
{
	if (mDuration < 0 or !mTarget) { mIsDestroy = true; return; }
	else
	{
		mDuration -= dTime;

		for (int i = 0; i < mTick; i++)
		{
			if (mDuration > i and mDuration <= i + dTime)
			{
				mTarget->Damage(mDamage);
				new Effect(L"Burning", mTarget->GetX(), mTarget->GetY(), EffectType::Normal);
			}
		}
	}


}

void Burning::Render(HDC hdc)
{
}
