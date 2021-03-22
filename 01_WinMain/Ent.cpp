#include "pch.h"
#include "Ent.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "Animation.h"

Ent::Ent(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"Ent", Resources(L"/skul/skul_ent.bmp"), 2400, 6000, 8, 20, true);
	mImage = IMAGEMANAGER->FindImage(L"Ent");

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}
void Ent::Init()
{
	mTileSelect = new TileSelect;

	mAnimationList[M rightIdle] = new Animation(0, 0, 3, 0, false, true, 0.2f);
	mAnimationList[M leftIdle] = new Animation(0, 1, 3, 1, false, true, 0.2f);
	mAnimationList[M rightWalk] = new Animation(0, 2, 7, 2, false, true, 0.2f);
	mAnimationList[M leftWalk] = new Animation(0, 3, 7, 3, false, true, 0.2f);

	mAnimationList[M rightDash] = new Animation(0, 4, 0, 4, false, false, 0.2f);
	mAnimationList[M leftDash] = new Animation(0, 5, 0, 5, false, false, 0.2f);

	mAnimationList[M rightAttack1] = new Animation(0, 8, 5, 8, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M rightAttack2] = new Animation(0, 10, 4, 10, false, false, mAttackSpeed);
	mAnimationList[M leftAttack1] = new Animation(0, 9, 5, 9, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M leftAttack2] = new Animation(0, 11, 4, 11, false, false, mAttackSpeed);

	mAnimationList[M rightSkill1] = new Animation(0, 12, 3, 12, false, false, 0.1f,
		[this]() {
			if (mSkill1CoolTime > 10.5f)
			{
				if (RIGHT) SetAnimation(M rightSkill1);
				if (LEFT) SetAnimation(M leftSkill1);
			}
		});
	mAnimationList[M leftSkill1] = new Animation(0, 13, 3, 13, false, false, 0.1f,
		[this]() {
			if (mSkill1CoolTime > 10.5f)
			{
				if (RIGHT) SetAnimation(M rightSkill1);
				if (LEFT) SetAnimation(M leftSkill1);
			}
		});

	mAnimationList[M rightCharging] = new Animation(0, 14, 3, 14, false, false, 0.25f,
		[this]() {
			mSkill2CoolTime = 13;
			if (RIGHT) SetAnimation(M rightSkill2Full);
			if (LEFT) SetAnimation(M leftSkill2Full);
		});
	mAnimationList[M leftCharging] = new Animation(0, 15, 3, 15, false, false, 0.25f,
		[this]() {
			mSkill2CoolTime = 13;
			if (RIGHT) SetAnimation(M rightSkill2Full);
			if (LEFT) SetAnimation(M leftSkill2Full);
		});

	mAnimationList[M rightSkill2] = new Animation(0, 16, 5, 16, false, false, 0.1f);
	mAnimationList[M leftSkill2] = new Animation(0, 17, 5, 17, false, false, 0.1f);
	mAnimationList[M rightSkill2Full] = new Animation(0, 18, 6, 18, false, false, 0.1f);
	mAnimationList[M leftSkill2Full] = new Animation(0, 19, 6, 19, false, false, 0.1f);

	mAnimationList[M rightSwitching] = new Animation(0,6,5,6,false,false,0.1f);
	mAnimationList[M leftSwitching] = new Animation(0,7,5,7,false,false,0.1f);

	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
}

void Ent::Update()
{
	if (LEFT and mPath.size() == 0) SetAnimation(M leftIdle);
	if (RIGHT and mPath.size() == 0) SetAnimation(M rightIdle);

	if (LEFT and mPath.size() > 0) SetAnimation(M leftWalk);
	if (RIGHT and mPath.size() > 0) SetAnimation(M rightWalk);

	mSpeed = mInitSpeed;
	if (TILE[mIndexY][mIndexX]->GetType() == TileType::Slow)
	{
		mSpeed = mInitSpeed / 2;
	}

	mTileSelect->Update();

	mDashCoolTime -= dTime;
	if (mDashCoolTime < 0) mDashCoolTime = 0;

	if (INPUT->GetKeyDown('Z')) //�뽬_ ��Ʈ�� ���� �뽬�� �Ұ��ϰ� �뽬 �� ���������� �ִ�
	{
		if (mDashCoolTime == 0)
		{
			Dash(5);
			Attack(1, 5, AttackType::Stab);
			if (LEFT) SetAnimation(M leftDash);
			if (RIGHT) SetAnimation(M rightDash);
			mDashCount = 1;
			mDashCoolTime = mInitDashCoolTime;
		}
	}

	if (mIsDash)
	{
		if (mCurrentAnimation != mAnimationList[M rightSwitching] and mCurrentAnimation != mAnimationList[M leftSwitching])
		Move(5 * mInitSpeed);
	}
	else
	{
		if(mCurrentAnimation!= mAnimationList[M rightSwitching] and mCurrentAnimation != mAnimationList[M leftSwitching])
		if (Input::GetInstance()->GetKey(VK_RBUTTON) and mTileSelect)
		{
			if (PathFinder::GetInstance()->FindPath(TILE, mPath, mIndexX, mIndexY,
				mTileSelect->GetIndexX(), mTileSelect->GetIndexY())) mPathIndex = 1;
		}
		Move(mSpeed);
	}

	if (INPUT->GetKey('X'))
	{
		mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
		if (RIGHT) { SetAnimation(M rightAttack1); }
		if (LEFT) { SetAnimation(M leftAttack1); }
	}
	BasicAttack();

	if (INPUT->GetKeyDown('A')) // ����
	{
		if (mSkill1CoolTime == 0)
		{
			mSkill1CoolTime =11;
			mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
			if (RIGHT) { SetAnimation(M rightSkill1); }
			if (LEFT) { SetAnimation(M leftSkill1); }
		}
		else
		{
			CAMERA->PanningOn(3);
		}
	}
	Skill1();

	if (INPUT->GetKeyDown('S')) // ��¡
	{
		if (mSkill2CoolTime == 0)
		{
			mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
			if (RIGHT) { SetAnimation(M rightCharging); }
			if (LEFT) { SetAnimation(M leftCharging); }
		}
		else
		{
			CAMERA->PanningOn(3);
		}
	}
	if (INPUT->GetKeyUp('S'))
	{
		if (mAnimationList[M rightCharging]->GetIsPlay() or mAnimationList[M leftCharging]->GetIsPlay())
		{
			mSkill2CoolTime = 13;

			if (mCurrentAnimation->GetNowFrameX() < 3)
			{
				mCurrentAnimation->Stop();
				if (RIGHT) mCurrentAnimation = mAnimationList[M rightSkill2];
				if (LEFT) mCurrentAnimation = mAnimationList[M leftSkill2];
				mCurrentAnimation->Play();
			}
		}
	}
	Skill2();

	SwitchAttack();

	mCurrentAnimation->Update();

	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	mHitBox = RectMakeBottom(mX, mY, 30, 30);



}

void Ent::Release()
{
	SafeDelete(mTileSelect);

	for (Animation* elem : mAnimationList)
	{
		SafeDelete(elem);
	}
}

void Ent::Render(HDC hdc)
{
	CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top + 25, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);

	mTileSelect->Render(hdc);

	//{{ �����ڿ� Ÿ�� üũ ������
	//TILE[mIndexY][mIndexX]->SelectRender(hdc);
	//
	//for (Tile* elem : mPath)
	//{
	//	elem->SelectRender(hdc);
	//}
	//}}
}

void Ent::SetAnimation(int listNum)
{
	if (mAnimationList[M rightDash]->GetIsPlay()) return;
	if (mAnimationList[M leftDash]->GetIsPlay()) return;

	if (mAnimationList[M rightAttack1]->GetIsPlay()) return;
	if (mAnimationList[M rightAttack2]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack1]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack2]->GetIsPlay()) return;

	if (mAnimationList[M leftSkill1]->GetIsPlay()) return;
	if (mAnimationList[M leftSkill2]->GetIsPlay()) return;
	if (mAnimationList[M rightSkill1]->GetIsPlay()) return;
	if (mAnimationList[M rightSkill2]->GetIsPlay()) return;

	if (mAnimationList[M rightSkill2Full]->GetIsPlay()) return;
	if (mAnimationList[M leftSkill2Full]->GetIsPlay()) return;
	if (mAnimationList[M rightCharging]->GetIsPlay()) return;
	if (mAnimationList[M leftCharging]->GetIsPlay()) return;

	if (mAnimationList[M rightSwitching]->GetIsPlay()) return;
	if (mAnimationList[M leftSwitching]->GetIsPlay()) return;

	mCurrentAnimation = mAnimationList[listNum];
	mCurrentAnimation->Play();
}

void Ent::BasicAttack()
{
	if (mAnimationList[M rightAttack1]->GetIsPlay() or mAnimationList[M leftAttack1]->GetIsPlay())
	{
		if (mCurrentAnimation->GetNowFrameX() == 3 and mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			Attack(1, 1, AttackType::Side);
		}
	}
	if (mAnimationList[M rightAttack2]->GetIsPlay() or mAnimationList[M leftAttack2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetNowFrameX() == 2 and mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			Attack(1, 1, AttackType::Side);
		}
	}

}

void Ent::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) mSkill1CoolTime = 0;

	if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
	{
		
		if (mCurrentAnimation->GetNowFrameX() == 1 and mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			Dash(2);
			Attack(1, 1, AttackType::Whirlwind);
			CAMERA->PanningOn(5);
		}
		if (mCurrentAnimation->GetNowFrameX() == 3 and mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			Dash(2);
			Attack(1, 1, AttackType::Whirlwind);
		}
	}
}

void Ent::Skill2()
{
	mSkill2CoolTime -= dTime;
	if (mSkill2CoolTime < 0) mSkill2CoolTime = 0;

	if (mAnimationList[M rightSkill2]->GetIsPlay() or mAnimationList[M leftSkill2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime and mCurrentAnimation->GetNowFrameX() == 1)
		{
			Attack(1, 2, AttackType::Whirlwind);
		}
	}
	else if (mAnimationList[M rightSkill2Full]->GetIsPlay() or mAnimationList[M leftSkill2Full]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime and mCurrentAnimation->GetNowFrameX() == 1)
		{
			Attack(2, 4, AttackType::Whirlwind);
		}
	}

}

void Ent::SkulSwitch(int indexX, int indexY)
{
	Player::SkulSwitch(indexX, indexY);
	if (LEFT)
	{
		SetAnimation(M leftSwitching);
	}
	if (RIGHT)
	{
		SetAnimation(M rightSwitching);
	}
}

void Ent::SkulReset()
{
	mCurrentAnimation->Stop();
}

void Ent::SwitchAttack()
{
	if (mAnimationList[M rightSwitching]->GetIsPlay() or mAnimationList[M leftSwitching]->GetIsPlay())
	{
		mInvincibility = true;

		if (mCurrentAnimation->GetNowFrameX() == 0)
		{
			mCurrentAnimation->SetFrameUpdateTime(1.f);
		}
		else
		{
			mCurrentAnimation->SetFrameUpdateTime(0.1f);
		}

		if (mCurrentAnimation->GetNowFrameX() == 1 and mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			Attack(1, 2, AttackType::Whirlwind); mInvincibility = false;
		}
	}
	
}


void Ent::SetAttackSpeed()
{
	mAnimationList[M rightAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M rightAttack2]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack2]->SetFrameUpdateTime(mAttackSpeed);
}