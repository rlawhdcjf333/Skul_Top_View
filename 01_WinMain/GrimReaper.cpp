#include "pch.h"
#include "GrimReaper.h"
#include "Animation.h"
#include "TileSelect.h"

GrimReaper::GrimReaper(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)

{
	IMAGEMANAGER->LoadFromFile(L"GrimReaper", Resources(L"/skul/skul_grim_reaper.bmp"), 3600, 3000, 12, 20, true);
	mImage = IMAGEMANAGER->FindImage(L"GrimReaper");

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

}

void GrimReaper::Init()
{
	mTileSelect = new TileSelect;
	
	mAnimationList[M rightIdle] = new Animation(0, 0, 5, 0, false, true, 0.2f);
	mAnimationList[M leftIdle] = new Animation(0, 1, 5, 1, false, true, 0.2f);
	mAnimationList[M rightWalk] = new Animation(0, 2, 5, 2, false, true, 0.2f);
	mAnimationList[M leftWalk] = new Animation(0, 3, 5, 3, false, true, 0.2f);

	mAnimationList[M rightDash] = new Animation(0, 4, 5, 4, false, false, 0.1f);
	mAnimationList[M leftDash] = new Animation(0, 5, 5, 5, false, false, 0.1f);

	mAnimationList[M rightAttack1] = new Animation(0, 8, 8, 8, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				UpdateAngle();
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M rightAttack2] = new Animation(0, 10, 8, 10, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				UpdateAngle();
				if (RIGHT) SetAnimation(M rightAttack3);
				if (LEFT) SetAnimation(M leftAttack3);
			}
		});
	mAnimationList[M rightAttack3] = new Animation(0, 12, 8, 12, false, false, mAttackSpeed, [this]() {mCurrentAnimation = mAnimationList[M rightIdle]; });
	mAnimationList[M leftAttack1] = new Animation(0, 9, 8, 9, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				UpdateAngle();
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M leftAttack2] = new Animation(0, 11, 8, 11, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				UpdateAngle();
				if (RIGHT) SetAnimation(M rightAttack3);
				if (LEFT) SetAnimation(M leftAttack3);
			}
		});
	mAnimationList[M leftAttack3] = new Animation(0, 13, 8, 13, false, false, mAttackSpeed, [this]() {mCurrentAnimation = mAnimationList[M leftIdle]; });

	mAnimationList[M rightSwitching] = new Animation(0,6,9,6, false, false, mAttackSpeed);
	mAnimationList[M leftSwitching] = new Animation(0,7,9,7,false, false, mAttackSpeed);

	mAnimationList[M rightSkill1] = new Animation(0,16,6,16,false, false, 0.1f);
	mAnimationList[M leftSkill1] = new Animation(0,17,6,17,false, false, 0.1f);
	mAnimationList[M rightSkill2] = new Animation(0,14,11,14,false, false, 0.2f);
	mAnimationList[M leftSkill2] = new Animation(0,15,11,15, false, false, 0.2f);


	mMagicalAttackPower = 3;
	
	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
}

void GrimReaper::Update()
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
	if (mDashCoolTime < 0) {
		mDashCoolTime = 0; mDashCount = 0;}

	if (!mAnimationList[M leftSkill2]->GetIsPlay() and !mAnimationList[M rightSkill2]->GetIsPlay())
	{
		if (INPUT->GetKeyDown('Z')) //대쉬
		{
				if (mDashCoolTime == 0)
				{
					mCurrentAnimation->Stop();
					Dash(5);
					if (LEFT) SetAnimation(M leftDash);
					if (RIGHT) SetAnimation(M rightDash);
					mDashCount = 1;
					mDashCoolTime = mInitDashCoolTime;
				}
				else if (mAnimationList[M leftDash]->GetIsPlay() or mAnimationList[M rightDash]->GetIsPlay())
				{

					if (mDashCount == 1)
					{
						mCurrentAnimation->Stop();
						Dash(5);
						if (LEFT) SetAnimation(M leftDash);
						if (RIGHT) SetAnimation(M rightDash);
						mDashCount = 0;
					}
				}
			
		}

		if (mIsDash)
		{
			Move(5 * mInitSpeed);
		}
		else
		{
			if (Input::GetInstance()->GetKey(VK_RBUTTON) and mTileSelect)
			{
				if (PathFinder::GetInstance()->FindPath(TILE, mPath, mIndexX, mIndexY,
					mTileSelect->GetIndexX(), mTileSelect->GetIndexY())) mPathIndex = 1;
			}
			Move(mSpeed);
		}
	}

	if (INPUT->GetKey('X'))
	{
		mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
		if (RIGHT) { SetAnimation(M rightAttack1); }
		if (LEFT) { SetAnimation(M leftAttack1); }
	}
	BasicAttack();

	if (INPUT->GetKeyDown('A')) // 선고
	{
		if (mSkill1CoolTime == 0)
		{
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

	if (INPUT->GetKeyDown('S')) // 수확
	{
		if (mSkill2CoolTime == 0)
		{
			mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
			if (RIGHT) { SetAnimation(M rightSkill2); }
			if (LEFT) { SetAnimation(M leftSkill2); }
		}
		else
		{
			CAMERA->PanningOn(3);
		}
	}
	Skill2();

	SwitchAttack();

	mCurrentAnimation->Update();

	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	mHitBox = RectMakeBottom(mX, mY, 30, 30);
}

void GrimReaper::Release()
{
	SafeDelete(mTileSelect);

	for (Animation* elem : mAnimationList)
	{
		SafeDelete(elem);
	}
}

void GrimReaper::Render(HDC hdc)
{
	CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top + 25, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);

	mTileSelect->Render(hdc);

}

void GrimReaper::SetAnimation(int listNum)
{
	if (mAnimationList[M rightDash]->GetIsPlay()) return;
	if (mAnimationList[M leftDash]->GetIsPlay()) return;

	if (mAnimationList[M rightAttack1]->GetIsPlay()) return;
	if (mAnimationList[M rightAttack2]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack1]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack2]->GetIsPlay()) return;
	if (mAnimationList[M rightAttack3]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack3]->GetIsPlay()) return;

	if (mAnimationList[M rightSwitching]->GetIsPlay()) return;
	if (mAnimationList[M leftSwitching]->GetIsPlay()) return;

	if (mAnimationList[M leftSkill1]->GetIsPlay()) return;
	if (mAnimationList[M leftSkill2]->GetIsPlay()) return;
	if (mAnimationList[M rightSkill1]->GetIsPlay()) return;
	if (mAnimationList[M rightSkill2]->GetIsPlay()) return;

	mCurrentAnimation = mAnimationList[listNum];
	mCurrentAnimation->Play();
}

void GrimReaper::SkulSwitch(int indexX, int indexY)
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

void GrimReaper::SkulReset()
{
	mCurrentAnimation->Stop();
}

void GrimReaper::BasicAttack()
{
	if (mAnimationList[M rightAttack1]->GetIsPlay() or mAnimationList[M rightAttack2]->GetIsPlay()
		or mAnimationList[M leftAttack1]->GetIsPlay() or mAnimationList[M leftAttack2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetNowFrameX() == 3 and mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			Attack(mMagicalAttackPower, 2, AttackType::Side);
		}
	}
	else if (mAnimationList[M rightAttack3]->GetIsPlay() or mAnimationList[M leftAttack3]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			if (mCurrentAnimation->GetCurrentFrameIndex() < 3)
			{
				Attack(mMagicalAttackPower, 1, AttackType::Whirlwind);
			}

			if (mCurrentAnimation->GetNowFrameX() == 3)
			{
				Attack(mMagicalAttackPower, 2, AttackType::Side);
			}

		}
	}
}

void GrimReaper::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) mSkill1CoolTime = 0;

	if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
	{
		mSkill1CoolTime = 12;

		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			if (mCurrentAnimation->GetCurrentFrameIndex() == 0)
			{
				Attack(5*mMagicalAttackPower, 5, AttackType::Stab);
				Dash(5);
				CAMERA->PanningOn(3);
			}
		}
	}
}

void GrimReaper::Skill2()
{
	mSkill2CoolTime -= dTime;
	if (mSkill2CoolTime < 0) mSkill2CoolTime = 0;

	if (mAnimationList[M rightSkill2]->GetIsPlay() or mAnimationList[M leftSkill2]->GetIsPlay())
	{
		mSkill2CoolTime = 45;

		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			if (mCurrentAnimation->GetCurrentFrameIndex()==0)
			{
				Obj->SetTimeStop(true);
			}
			if (mCurrentAnimation->GetCurrentFrameIndex() == 11)
			{
				Obj->SetTimeStop(false);
				Attack(10 * mMagicalAttackPower, 15, AttackType::Whirlwind);
			}
		}
	}
}

void GrimReaper::SwitchAttack()
{
	if (mAnimationList[M leftSwitching]->GetIsPlay() or mAnimationList[M rightSwitching]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			switch (mCurrentAnimation->GetCurrentFrameIndex())
			{
			case 0:
			case 1:
			case 2:
				SKUL->Invincibilize();
				break;
			case 3:
				Attack(mMagicalAttackPower, 3, AttackType::Whirlwind);
				SKUL->Disinvincibilize();

			default: 
				break;
			}
		}
	}
}

void GrimReaper::SetAttackSpeed()
{
	mAnimationList[M rightAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M rightAttack2]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack2]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack3]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M rightAttack3]->SetFrameUpdateTime(mAttackSpeed);
}
