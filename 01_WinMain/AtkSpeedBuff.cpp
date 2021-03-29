#include "pch.h"
#include "AtkSpeedBuff.h"

AtkSpeedBuff::AtkSpeedBuff(int percentage, float duration, string name)
{
	mName = name;

	if (mName == "BerserkerAtkSpeedBuff" and Obj->FindObject(ObjectLayer::Condition, "BerserkerAtkSpeedBuff")) //�̹� �����ϴ� ������� ����, ��ø������ ���ĥ ���� ��
	{
		dynamic_cast<Condition*>(Obj->FindObject(ObjectLayer::Condition, "BerserkerAtkSpeedBuff"))->SetDuration(duration);
		delete (this);
	}
	else
	{
		mIsActive = true;
		mIsDestroy = false;

		mDuration = duration;

		mValue = (float)(mAttackSpeed * percentage) / (float)(100 + percentage);

		SKUL->SetAtkSpeed(mAttackSpeed - mValue);
		SKUL->GetCurrentSkul()->SetAttackSpeed();
		if (SKUL->GetAlterSkul()) SKUL->GetAlterSkul()->SetAttackSpeed();
		Obj->AddObject(ObjectLayer::Condition, this);
	}
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
