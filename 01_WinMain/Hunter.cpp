#include "pch.h"
#include "Hunter.h"
#include "TileSelect.h"
#include "Animation.h"
#include "Arrow.h" //화살 이미지 개선, 반향 잔상효과 적용

Hunter::Hunter(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"Hunter", Resources(L"/skul/skul_hunter.bmp"), 700, 1000, 7, 10, true);
	mImage = IMAGEMANAGER->FindImage(L"Hunter");

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}
void Hunter::Init()
{
	mTileSelect = new TileSelect;

	mAnimationList[M rightIdle] = new Animation(0, 0, 3, 0, false, true, 0.2f);
	mAnimationList[M leftIdle] = new Animation(0, 1, 3, 1, true, true, 0.2f);
	mAnimationList[M rightWalk] = new Animation(0, 2, 6, 2, false, true, 0.2f);
	mAnimationList[M leftWalk] = new Animation(0, 3, 6, 3, true, true, 0.2f);

	mAnimationList[M rightDash] = new Animation(0, 4, 6, 4, false, false, 0.05f);
	mAnimationList[M leftDash] = new Animation(0, 5, 6, 5, true, false, 0.05f);

	mAnimationList[M rightAttackCharging] = new Animation(0, 6, 4, 6, false, true, mAttackSpeed, [this]() {mCurrentAnimation->SetCurrentFrameIndex(3);});
	mAnimationList[M leftAttackCharging] = new Animation(2, 7, 6, 7, true, true, mAttackSpeed, [this]() {mCurrentAnimation->SetCurrentFrameIndex(3);});
	mAnimationList[M rightAttack1] = new Animation(5, 6, 6, 6, false, false, mAttackSpeed);
	mAnimationList[M leftAttack1] = new Animation(0, 7, 1, 7, true, false, mAttackSpeed);

	mAnimationList[M rightSkill1] = new Animation(0, 8, 6, 8, false, false, 0.1f);
	mAnimationList[M leftSkill1] = new Animation(0, 9, 6, 9, true, false, 0.1f);
	mAnimationList[M rightSkill2] = new Animation(0, 8, 6, 8, false, false, 0.1f);
	mAnimationList[M leftSkill2] = new Animation(0, 9, 6, 9, true, false, 0.1f);

	mAnimationList[M leftSwitching] = new Animation(0, 4, 6, 4, false, false, 0.05f);
	mAnimationList[M rightSwitching] = new Animation(0, 5, 6, 5, true, false, 0.05f);

	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
}

void Hunter::Update()
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

	mDashCoolTime -= dTime;
	if (mDashCoolTime < 0) { mDashCoolTime = 0; mDashCount = 0; }

	if (INPUT->GetKeyDown('Z')) //대쉬
	{
		if (mDashCoolTime == 0)
		{
			mCurrentAnimation->Stop();
			Dash(3);
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
				Dash(3);
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

	if (INPUT->GetKeyDown('X'))
	{
		UpdateAngle();
		if (RIGHT) { SetAnimation(M rightAttackCharging); }
		if (LEFT) { SetAnimation(M leftAttackCharging); }
	}
	BasicAttack();

	if (INPUT->GetKeyDown('A')) // 멀티플
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

	if (INPUT->GetKeyDown('S')) //  피어스
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

void Hunter::Release()
{
	SafeDelete(mTileSelect);

	for (Animation* elem : mAnimationList)
	{
		SafeDelete(elem);
	}
}

void Hunter::Render(HDC hdc)
{
	CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top + 25, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);

	mTileSelect->Render(hdc);

	//{{ 개발자용 타일 체크 렌더링
	//TILE[mIndexY][mIndexX]->SelectRender(hdc);
	//
	//for (Tile* elem : mPath)
	//{
	//	elem->SelectRender(hdc);
	//}
	//}}
}

void Hunter::SetAnimation(int listNum)
{
	if (mAnimationList[M rightDash]->GetIsPlay()) return;
	if (mAnimationList[M leftDash]->GetIsPlay()) return;

	if (mAnimationList[M rightAttack1]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack1]->GetIsPlay()) return;
	if (mAnimationList[M rightAttackCharging]->GetIsPlay() or mAnimationList[M leftAttackCharging]->GetIsPlay())
	{
		mPath.clear();
		return;
	}

	if (mAnimationList[M leftSkill1]->GetIsPlay()) return;
	if (mAnimationList[M leftSkill2]->GetIsPlay()) return;
	if (mAnimationList[M rightSkill1]->GetIsPlay()) return;
	if (mAnimationList[M rightSkill2]->GetIsPlay()) return;

	if (mAnimationList[M rightSwitching]->GetIsPlay()) return;
	if (mAnimationList[M leftSwitching]->GetIsPlay()) return;

	mCurrentAnimation = mAnimationList[listNum];
	mCurrentAnimation->Play();
}

void Hunter::BasicAttack()
{
	if (INPUT->GetKeyUp('X'))
	{
		if (mAnimationList[M rightAttackCharging]->GetIsPlay() or mAnimationList[M leftAttackCharging]->GetIsPlay())
		{
			UpdateAngle();
			switch (mCurrentAnimation->GetCurrentFrameIndex())
			{
			case 3:
			case 4:
				new Arrow(this, 2*mPhysicalAttackPower, mAngle, true); //헌터 패시브 적용 차징샷에 피어싱 효과 부여 및 사거리/ 속도 증가
				break;

			default:
				new Arrow(this, 2*mPhysicalAttackPower, mAngle, false);
				break;
			}
			mCurrentAnimation->Stop();
			if (RIGHT) { SetAnimation(M rightAttack1); }
			if (LEFT) { SetAnimation(M leftAttack1); }
		}
	}
}

void Hunter::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) mSkill1CoolTime = 0;

	if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
	{
		mSkill1CoolTime = 8;

		if (mCurrentAnimation->GetCurrentFrameTime() >0.1f-dTime and mCurrentAnimation->GetCurrentFrameIndex() == 3)
		{
			for (int i = 0; i < 5; i++)
			new Arrow(this, 3*mPhysicalAttackPower, mAngle-PI/6+i*PI/12, false);

			CAMERA->PanningOn(5);

		}
	}
}

void Hunter::Skill2()
{
	mSkill2CoolTime -= dTime;
	if (mSkill2CoolTime < 0) mSkill2CoolTime = 0;

	if (mAnimationList[M rightSkill2]->GetIsPlay() or mAnimationList[M leftSkill2]->GetIsPlay())
	{
		mSkill2CoolTime = 14;

		if (mCurrentAnimation->GetCurrentFrameTime() > 0.1f-dTime)
		{
			if (mCurrentAnimation->GetNowFrameX() == 3)
			{
				new Arrow(this, 3*mMagicalAttackPower, mAngle, true);
				CAMERA->PanningOn(5);
			}
		}
	}
}

void Hunter::SwitchAttack()
{
	if (mAnimationList[M rightSwitching]->GetIsPlay() or mAnimationList[M leftSwitching]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() > 0.05f - dTime)
		{
			if (mCurrentAnimation->GetCurrentFrameIndex() == 6)
			{
				UpdateAngle();
				mCurrentAnimation->Stop();
				if (LEFT)
				{
					SetAnimation(M leftAttack1);
					new Arrow(this, 3 * mPhysicalAttackPower, mAngle, true);
				}
				else if (RIGHT)
				{
					SetAnimation(M rightAttack1);
					new Arrow(this, 3 * mPhysicalAttackPower, mAngle, true);
				}
			}
		}
	}

}

void Hunter::SkulSwitch(int indexX, int indexY)
{
	Player::SkulSwitch(indexX, indexY);
	if (LEFT)
	{
		Dash(3, true);
		SetAnimation(M leftSwitching);
	}
	if (RIGHT)
	{
		Dash(3, true);
		SetAnimation(M rightSwitching);
	}
}

void Hunter::SkulReset()
{
	mCurrentAnimation->Stop();
}

void Hunter::SetAttackSpeed()
{
	mAnimationList[M rightAttackCharging]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttackCharging]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M rightAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack1]->SetFrameUpdateTime(mAttackSpeed);
}
