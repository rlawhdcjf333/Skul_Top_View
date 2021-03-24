#include "pch.h"
#include "Mino.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "Animation.h"

Mino::Mino(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"Mino", Resources(L"/skul/skul_mino.bmp"), 800, 1100, 8, 11, true);
	mImage = IMAGEMANAGER->FindImage(L"Mino");

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}
void Mino::Init()
{
	mTileSelect = new TileSelect;

	mAnimationList[M rightIdle] = new Animation(0, 0, 4, 0, false, true, 0.2f);
	mAnimationList[M leftIdle] = new Animation(0, 1, 4, 1, true, true, 0.2f);
	mAnimationList[M rightWalk] = new Animation(0, 2, 7, 2, false, true, 0.2f);
	mAnimationList[M leftWalk] = new Animation(0, 3, 7, 3, true, true, 0.2f);

	mAnimationList[M rightDash] = new Animation(0, 4, 0, 4, false, false, 0.35f);
	mAnimationList[M leftDash] = new Animation(1, 4, 1, 4, false, false, 0.35f);

	mAnimationList[M rightAttack1] = new Animation(0, 5, 5, 5, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M rightAttack2] = new Animation(0, 7, 4, 7, false, false, mAttackSpeed);
	mAnimationList[M leftAttack1] = new Animation(0, 6, 5, 6, true, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M leftAttack2] = new Animation(0, 8, 4, 8, true, false, mAttackSpeed);

	mAnimationList[M rightSkill1] = new Animation(0, 7, 4, 7, false, false, 0.05f);
	mAnimationList[M leftSkill1] = new Animation(0, 8, 4, 8, true, false, 0.05f);
	mAnimationList[M rightSkill2] = new Animation(0, 4, 0, 4, false, false, 0.2f,
		[this]() {
			if (mSkill2CoolTime > 13.f)
			{
				if (RIGHT) SetAnimation(M rightSkill2);
				if (LEFT) SetAnimation(M leftSkill2);
				Dash(2);
				Attack(1, 2, AttackType::Side);
				CAMERA->PanningOn(5);
			}
		});
	mAnimationList[M rightSkill2CancelAttack] = new Animation(0, 7, 4, 7, false, false, 0.1f);
	mAnimationList[M leftSkill2] = new Animation(1, 4, 1, 4, false, false, 0.2f,
		[this]() {
			if (mSkill2CoolTime > 13.f)
			{
				if (RIGHT) SetAnimation(M rightSkill2);
				if (LEFT) SetAnimation(M leftSkill2);
				Dash(2);
				Attack(1, 2, AttackType::Side);
				CAMERA->PanningOn(5);
			}
		});
	mAnimationList[M leftSkill2CancelAttack] = new Animation(0, 8, 4, 8, true, false, 0.1f);

	mAnimationList[M rightSwitching] = new Animation(0,5,5,5,false,false,0.1f);
	mAnimationList[M leftSwitching] = new Animation(0,6,5,6,true, false, 0.1f);

	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
}

void Mino::Update()
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
	if (INPUT->GetKeyDown('Z')) //대쉬
	{
		if (mDashCoolTime == 0)
		{
			mCurrentAnimation->Stop();
			Dash(5);
			Attack(1, 5, AttackType::Stab);
			if (LEFT) SetAnimation(M leftDash);
			if (RIGHT) SetAnimation(M rightDash);
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
		mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
		if (RIGHT) { SetAnimation(M rightAttack1); }
		if (LEFT) { SetAnimation(M leftAttack1); }
	}
	BasicAttack();
	
	if (INPUT->GetKeyDown('A')) // 돌진
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
	

	if (INPUT->GetKeyDown('S')) //  돌진 몽둥이
	{
		if (mSkill2CoolTime == 0)
		{
			mCurrentAnimation->Stop();
			mSkill2CoolTime = 14;
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

	
	mCurrentAnimation->Update();
	
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	mHitBox = RectMakeBottom(mX, mY, 30, 30);

}

void Mino::Release()
{
	SafeDelete(mTileSelect);

	for (Animation* elem : mAnimationList)
	{
		SafeDelete(elem);
	}
}

void Mino::Render(HDC hdc)
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

void Mino::SetAnimation(int listNum)
{
	if (mAnimationList[M rightDash]->GetIsPlay()) return;
	if (mAnimationList[M leftDash]->GetIsPlay()) return;

	if (mAnimationList[M rightAttack1]->GetIsPlay()) return;
	if (mAnimationList[M rightAttack2]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack1]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack2]->GetIsPlay()) return;

	if (mAnimationList[M leftSkill1]->GetIsPlay()) return;
	if (mAnimationList[M rightSkill1]->GetIsPlay()) return;
	if (mAnimationList[M leftSkill2]->GetIsPlay()) return;
	if (mAnimationList[M rightSkill2]->GetIsPlay()) return;

	if (mAnimationList[M rightSwitching]->GetIsPlay()) return;
	if (mAnimationList[M leftSwitching]->GetIsPlay()) return;

	mCurrentAnimation = mAnimationList[listNum];
	mCurrentAnimation->Play();
}

void Mino::BasicAttack()
{
	if (mAnimationList[M rightAttack1]->GetIsPlay() or mAnimationList[M leftAttack1]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			if (mAnimationList[M rightAttack1]->GetNowFrameX() ==3)
			{
				Attack(1, 2, AttackType::Side);
			}
			else if (mAnimationList[M leftAttack1]->GetNowFrameX() ==2)
			{
				Attack(1, 2, AttackType::Side);
			}
		}
	}
	else if (mAnimationList[M rightAttack2]->GetIsPlay() or mAnimationList[M leftAttack2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetNowFrameX() == 2 and mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			Attack(1, 2, AttackType::Side);
		}
	}
}

void Mino::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) mSkill1CoolTime = 0;

	if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
	{
		mSkill1CoolTime = 10;
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			if (mCurrentAnimation->GetCurrentFrameIndex() == 1)
			{
				Dash(5);
				CAMERA->PanningOn(5);
			}
			else
			{
				Attack(mPhysicalAttackPower, 2, AttackType::Side);
			}
		}
	}
}

void Mino::Skill2()
{
	mSkill2CoolTime -= dTime;
	if (mSkill2CoolTime < 0) mSkill2CoolTime = 0;

	if (mAnimationList[M rightSkill2]->GetIsPlay() or mAnimationList[M leftSkill2]->GetIsPlay())
	{
		if (INPUT->GetKeyDown('S') and mSkill2CoolTime<13.8f)
		{
			mCurrentAnimation->Stop();
			if (RIGHT) mCurrentAnimation = mAnimationList[M rightAttack2];
			if (LEFT) mCurrentAnimation = mAnimationList[M rightAttack2];
			mCurrentAnimation->Play();
			Attack(mPhysicalAttackPower, 2, AttackType::Whirlwind);
			CAMERA->PanningOn(5);

			//기절 함수 필요
		}
	}
}

void Mino::SkulSwitch(int indexX, int indexY)
{
	Player::SkulSwitch(indexX, indexY);
	if (LEFT)
	{
		Attack(1, 2, AttackType::Side);
		SetAnimation(M leftSwitching);
	}
	if (RIGHT)
	{
		Attack(1, 2, AttackType::Side);
		SetAnimation(M rightSwitching);
	}
}

void Mino::SkulReset()
{
	mCurrentAnimation->Stop();
}

void Mino::SetAttackSpeed()
{
	mAnimationList[M rightAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M rightAttack2]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack2]->SetFrameUpdateTime(mAttackSpeed);
}
