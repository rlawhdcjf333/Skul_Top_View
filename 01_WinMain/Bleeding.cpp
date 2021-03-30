#include "pch.h"
#include "Bleeding.h"
#include "Effect.h"

Bleeding::Bleeding(GameObject* target, int damage, float duration)
{
	mName = "Bleeding";
	mIsActive = true;
	mIsDestroy = false;

	mDuration = duration;
	mTarget = target;
	mDamage = damage;

	mTick = (int)duration;
	Obj->AddObject(ObjectLayer::Condition, this);
}

void Bleeding::Update()
{
	if (mDuration < 0 or !mTarget or mTarget->GetIsDestroy()) { mIsDestroy = true; return; }
	else
	{
		mDuration -= dTime;

		for (int i = 0; i < mTick; i++)
		{
			if (mDuration > i  and mDuration <= i+ dTime)
			{
				if(mTarget) mTarget->Damage(mDamage);
				new Effect(L"Bleeding", mTarget->GetX(), mTarget->GetY(), EffectType::Normal);
			}
		}

	}

}

void Bleeding::Render(HDC hdc)
{
}
