#include "pch.h"
#include "Yaksha.h"
#include "Animation.h"
#include "TileSelect.h"

Yaksha::Yaksha(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)

{
	IMAGEMANAGER->LoadFromFile(L"Yaksha", Resources(L"/skul/skul_yaksha.bmp"), 1800, 3000, 12, 20, true);
	mImage = IMAGEMANAGER->FindImage(L"Yaksha");

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

}

void Yaksha::Init()
{
	mTileSelect = new TileSelect;

	mAnimationList[M rightIdle] = new Animation(0, 0, 6, 0, false, true, 0.2f);
	mAnimationList[M leftIdle] = new Animation(0, 1, 6, 1, false, true, 0.2f);
	mAnimationList[M rightWalk] = new Animation(0, 2, 7, 2, false, true, 0.2f);
	mAnimationList[M leftWalk] = new Animation(0, 3, 7, 3, false, true, 0.2f);

	mAnimationList[M rightDash] = new Animation(0, 4, 3, 4, false, false, 0.1f);
	mAnimationList[M leftDash] = new Animation(0, 5, 3, 5, false, false, 0.1f);

	mAnimationList[M rightAttack1] = new Animation(0, 6, 5, 6, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				UpdateAngle();
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M rightAttack2] = new Animation(0, 8, 9, 8, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				UpdateAngle();
				if (RIGHT) SetAnimation(M rightAttack3);
				if (LEFT) SetAnimation(M leftAttack3);
			}
		});
	mAnimationList[M rightAttack3] = new Animation(0, 10, 6, 10, false, false, mAttackSpeed, [this]() {mCurrentAnimation = mAnimationList[M rightIdle]; });
	mAnimationList[M leftAttack1] = new Animation(0, 7, 5, 7, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				UpdateAngle();
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M leftAttack2] = new Animation(0, 9, 9, 9, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				UpdateAngle();
				if (RIGHT) SetAnimation(M rightAttack3);
				if (LEFT) SetAnimation(M leftAttack3);
			}
		});
	mAnimationList[M leftAttack3] = new Animation(0, 11, 6, 11, false, false, mAttackSpeed, [this]() {mCurrentAnimation = mAnimationList[M leftIdle]; });

	mAnimationList[M rightSwitching] = new Animation(0, 6, 5, 6, false, false, mAttackSpeed);
	mAnimationList[M leftSwitching] = new Animation(0, 7, 5, 7, false, false, mAttackSpeed);

	mAnimationList[M rightCharging] = new Animation(0,12,4,12, false, false, 0.2f, [this]() {
		mCurrentAnimation->Play(), mCurrentAnimation->SetCurrentFrameIndex(4);
		});
	mAnimationList[M leftCharging] = new Animation(0,13,4,13,false, false, 0.2f, [this]() {
		mCurrentAnimation->Play(), mCurrentAnimation->SetCurrentFrameIndex(4);
		});
	mAnimationList[M rightSkill1] = new Animation(0, 14, 4, 14, false, false, 0.1f);
	mAnimationList[M leftSkill1] = new Animation(0, 15, 4, 15, false, false, 0.1f);
	mAnimationList[M rightSkill1Full] = new Animation(0,16,4,16, false, false, 0.1f);
	mAnimationList[M leftSkill1Full] = new Animation(0,17,4,17,false, false, 0.1f);

	mAnimationList[M rightSkill2] = new Animation(0, 18, 11, 18, false, false, 0.2f);
	mAnimationList[M leftSkill2] = new Animation(0, 19, 11, 19, false, false, 0.2f);


	mPhysicalAttackPower = 3;

	mStompCount = 0;
	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
}

void Yaksha::Update()
{
	if (LEFT and mPath.size() == 0) SetAnimation(M leftIdle);
	if (RIGHT and mPath.size() == 0) SetAnimation(M rightIdle);

	if (M_LEFT and mPath.size() > 0) { SetAnimation(M leftWalk); }
	if (M_RIGHT and mPath.size() > 0) { SetAnimation(M rightWalk); }

	mSpeed = mInitSpeed;
	if (TILE[mIndexY][mIndexX]->GetType() == TileType::Slow)
	{
		mSpeed = mInitSpeed / 2;
	}

	mTileSelect->Update();

	if (mStompCount > 4)
	{
		Attack(mPhysicalAttackPower, 3, AttackType::Side);
		mStompCount = 0;
	}



	mDashCoolTime -= dTime;
	if (mDashCoolTime < 0) {
		mDashCoolTime = 0; mDashCount = 0;
	}

	if (INPUT->GetKeyDown('Z')) //대쉬
	{
		if (mDashCoolTime == 0)
		{
			mCurrentAnimation->Stop();
			Dash(5);
			Attack(mPhysicalAttackPower, 5, AttackType::Stab);
			if (LEFT) SetAnimation(M leftDash);
			if (RIGHT) SetAnimation(M rightDash);
			mStompCount++ ;
			mDashCount = 1;
			mDashCoolTime = mInitDashCoolTime;
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

	if (INPUT->GetKey('X'))
	{
		if (RIGHT) { SetAnimation(M rightAttack1); }
		if (LEFT) { SetAnimation(M leftAttack1); }
	}
	BasicAttack();

	if (INPUT->GetKeyDown('A')) // 야차 정권
	{
		if (mSkill1CoolTime == 0)
		{
			UpdateAngle();
			if (RIGHT) { SetAnimation(M rightCharging); }
			if (LEFT) { SetAnimation(M leftCharging); }
		}
		else
		{
			CAMERA->PanningOn(3);
		}
	}
	if (INPUT->GetKeyUp('A'))
	{
		if (mAnimationList[M rightCharging]->GetIsPlay() or mAnimationList[M leftCharging]->GetIsPlay())
		{
			UpdateAngle();
			switch (mCurrentAnimation->GetCurrentFrameIndex())
			{
				case 0:
				case 1:
				case 2:
				case 3:
					mCurrentAnimation->Stop();
					if (RIGHT) { SetAnimation(M rightSkill1); }
					if (LEFT) { SetAnimation(M leftSkill1); }
					break;
				case 4:
					mCurrentAnimation->Stop();
					if (RIGHT) { SetAnimation(M rightSkill1Full); }
					if (LEFT) { SetAnimation(M leftSkill1Full); }
					break;
			}
		}
	}

	Skill1();

	if (INPUT->GetKeyDown('S')) // 야차 행진
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

void Yaksha::Release()
{
	SafeDelete(mTileSelect);

	for (Animation* elem : mAnimationList)
	{
		SafeDelete(elem);
	}
}

void Yaksha::Render(HDC hdc)
{
	CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top + 25, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);

	mTileSelect->Render(hdc);

	TextOut(hdc, 100, 100, to_wstring(mStompCount).c_str(), to_wstring(mStompCount).length());

}

void Yaksha::SetAnimation(int listNum)
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

	if (mAnimationList[M rightCharging]->GetIsPlay() or mAnimationList[M leftCharging]->GetIsPlay()) return;

	if (mAnimationList[M leftSkill1]->GetIsPlay()) return;
	if (mAnimationList[M leftSkill1Full]->GetIsPlay()) return;
	if (mAnimationList[M leftSkill2]->GetIsPlay()) return;

	if (mAnimationList[M rightSkill1]->GetIsPlay()) return;
	if (mAnimationList[M rightSkill1Full]->GetIsPlay()) return;
	if (mAnimationList[M rightSkill2]->GetIsPlay()) return;

	mCurrentAnimation = mAnimationList[listNum];
	mCurrentAnimation->Play();
}

void Yaksha::SkulSwitch(int indexX, int indexY)
{
	Player::SkulSwitch(indexX, indexY);
	mStompCount++;
	if (LEFT)
	{
		SetAnimation(M leftSwitching);
	}
	if (RIGHT)
	{
		SetAnimation(M rightSwitching);
	}
}

void Yaksha::SkulReset()
{
	mCurrentAnimation->Stop();
}

void Yaksha::BasicAttack()
{
	if (mAnimationList[M rightAttack1]->GetIsPlay() or mAnimationList[M leftAttack1]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameIndex() == 3 and mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			Attack(mPhysicalAttackPower, 3, AttackType::Side);
			mStompCount++;
		}
	}
	else if (mAnimationList[M rightAttack2]->GetIsPlay() or mAnimationList[M leftAttack2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetNowFrameX() == 6 and mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			Attack(mPhysicalAttackPower, 3, AttackType::Stab);
		}
	}
	else if (mAnimationList[M rightAttack3]->GetIsPlay() or mAnimationList[M leftAttack3]->GetIsPlay())
	{
		if (mCurrentAnimation->GetNowFrameX() == 2 and mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			Attack(mPhysicalAttackPower, 3, AttackType::Whirlwind);
		}
	}
}

void Yaksha::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) mSkill1CoolTime = 0;

	if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
	{
		mSkill1CoolTime = 1;

		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			if (mCurrentAnimation->GetCurrentFrameIndex() == 1)
			{
				Attack(5 * mPhysicalAttackPower, 3, AttackType::Stab);
				Dash(3);
				CAMERA->PanningOn(3);
			}
		}
	}
	else if (mAnimationList[M rightSkill1Full]->GetIsPlay() or mAnimationList[M leftSkill1Full]->GetIsPlay())
	{
		mSkill1CoolTime = 1;

		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			if (mCurrentAnimation->GetCurrentFrameIndex() == 1)
			{
				Attack(10 * mPhysicalAttackPower, 5, AttackType::Stab);
				Dash(5);
				CAMERA->PanningOn(5);
			}
		}
	}
	
}

void Yaksha::Skill2()
{
	mSkill2CoolTime -= dTime;
	if (mSkill2CoolTime < 0) mSkill2CoolTime = 0;

	if (mAnimationList[M rightSkill2]->GetIsPlay() or mAnimationList[M leftSkill2]->GetIsPlay())
	{
		mSkill2CoolTime = 8;

		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			switch (mCurrentAnimation->GetCurrentFrameIndex())
			{
			case 1:
			case 4:
			case 9:
				Attack(5 * mPhysicalAttackPower, 3, AttackType::Whirlwind);
				Dash(1);
				mStompCount++;
				CAMERA->PanningOn(5);
				break;
			default:
				break;
			}
		}
	}
}

void Yaksha::SwitchAttack()
{
	if (mAnimationList[M leftSwitching]->GetIsPlay() or mAnimationList[M rightSwitching]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			switch (mCurrentAnimation->GetCurrentFrameIndex())
			{
			case 4:
				Attack(mPhysicalAttackPower, 3, AttackType::Side);
				break;
			}
		}
	}
}

void Yaksha::SetAttackSpeed()
{
	mAnimationList[M rightAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M rightAttack2]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack2]->SetFrameUpdateTime(mAttackSpeed);
}
