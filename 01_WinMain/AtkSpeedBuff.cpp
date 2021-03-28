#include "pch.h"
#include "AtkSpeedBuff.h"

AtkSpeedBuff::AtkSpeedBuff(int percentage, float duration)
{
	mName = "AtkSpeedBuff";
	mIsActive = true;
	mIsDestroy = false;

	mDuration = duration;

	mValue = (mAttackSpeed * percentage) / (100 + percentage);
	if (mValue > 0.08f) mValue = 0.08f;

	SKUL->SetAtkSpeed(mAttackSpeed - mValue);
	SKUL->GetCurrentSkul()->SetAttackSpeed();
	if (SKUL->GetAlterSkul()) SKUL->GetAlterSkul()->SetAttackSpeed();

	Obj->AddObject(ObjectLayer::Condition, this);
}

void AtkSpeedBuff::Update()
{
	mDuration -= dTime;
	if (mDuration < 0) mIsDestroy = true;
}

void AtkSpeedBuff::Render(HDC hdc)
{
}

void AtkSpeedBuff::Release()
{
	SKUL->SetAtkSpeed(mAttackSpeed + mValue);
	SKUL->GetCurrentSkul()->SetAttackSpeed(); 
	if (SKUL->GetAlterSkul()) SKUL->GetAlterSkul()->SetAttackSpeed();
}
