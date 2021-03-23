#include "pch.h"
#include "HighWarlock.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "Animation.h"

HighWarlock::HighWarlock(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"HighWarlock", Resources(L"/skul/skul_high_warlock.bmp"), 3200, 3200, 16, 16, true);
	mImage = IMAGEMANAGER->FindImage(L"HighWarlock");

	IMAGEMANAGER->LoadFromFile(L"WarlockProjectile", Resources(L"/skul/warlockProjectile.bmp"), 384, 40, 12, 2, true);
	mWarlockProjectile = IMAGEMANAGER->FindImage(L"WarlockProjectile");

	IMAGEMANAGER->LoadFromFile(L"WarlockBarricade", Resources(L"/skul/warlockBarricade.bmp"), 1125, 1100, 5, 10, true);
	mWarlockBarricade = IMAGEMANAGER->FindImage(L"WarlockBarricade");

	IMAGEMANAGER->LoadFromFile(L"MeteorComp", Resources(L"/skul/meteorComp.bmp"), 2000, 1000, 4, 2, true);
	mMeteorComp = IMAGEMANAGER->FindImage(L"MeteorComp");

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}
void HighWarlock::Init()
{
	mTileSelect = new TileSelect;

	mAnimationList[M rightIdle] = new Animation(0, 0, 5, 0, false, true, 0.2f);
	mAnimationList[M leftIdle] = new Animation(0, 1, 5, 1, true, true, 0.2f);
	mAnimationList[M rightWalk] = new Animation(0, 2, 5, 2, false, true, 0.2f);
	mAnimationList[M leftWalk] = new Animation(0, 3, 5, 3, true, true, 0.2f);

	mAnimationList[M rightDash] = new Animation(0, 4, 1, 4, false, false, 0.2f);
	mAnimationList[M leftDash] = new Animation(0, 5, 1, 5, true, false, 0.2f);

	mAnimationList[M rightAttack1] = new Animation(0, 6, 6, 6, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M rightAttack2] = new Animation(0, 8, 3, 8, false, false, mAttackSpeed);
	mAnimationList[M leftAttack1] = new Animation(0, 7, 6, 7, true, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M leftAttack2] = new Animation(0, 9, 3, 9, true, false, mAttackSpeed);

	mAnimationList[M rightSkill1] = new Animation(0, 10, 15, 10, false, false, 0.1f);
	mAnimationList[M leftSkill1] = new Animation(0, 11, 15, 11, true, false, 0.1f);
	mAnimationList[M rightSkill2] = new Animation(0, 12, 7, 12, false, false, 0.1f);
	mAnimationList[M leftSkill2] = new Animation(0, 13, 7, 13, true, false, 0.1f);

	mMagicalAttackPower = 2;
	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
}

void HighWarlock::Update()
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

	if (INPUT->GetKeyDown('A')) //메테오
	{
		if (mSkill1CoolTime == 0)
		{
			mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
			if (RIGHT) { SetAnimation(M leftSkill1); }
			if (LEFT) { SetAnimation(M leftSkill1); }
		}
		else
		{
			CAMERA->PanningOn(3);
		}
	}
	if (INPUT->GetKeyUp('A'))
	{
		if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
		{
			if (mCurrentAnimation->GetCurrentFrameIndex() < 6)
			{

			}
			else
			{
				new Bullet(mMeteorComp, "MeteorComp", this, 10, 300, 1200, 0, BulletType::MeteorStrike);

			}

			mSkill1CoolTime = 20;
		}


	}
	Skill1();

	if (INPUT->GetKeyDown('S')) // 어비스 오브
	{
		if (mSkill2CoolTime == 0)
		{
			mSkill2CoolTime = 12;
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

void HighWarlock::Release()
{
	SafeDelete(mTileSelect);

	for (Animation* elem : mAnimationList)
	{
		SafeDelete(elem);
	}
}

void HighWarlock::Render(HDC hdc)
{
	CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top + 50, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);

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

void HighWarlock::SetAnimation(int listNum)
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

	mCurrentAnimation = mAnimationList[listNum];
	mCurrentAnimation->Play();
}

void HighWarlock::BasicAttack()
{
	if (mAnimationList[M rightAttack1]->GetIsPlay() or mAnimationList[M leftAttack1]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			if (mCurrentAnimation->GetCurrentFrameIndex()==2 or mCurrentAnimation->GetCurrentFrameIndex() == 4)
			{
				new Bullet(mWarlockProjectile, "WarlockProjectile", this, mMagicalAttackPower, 400,600, mAngle, BulletType::FrameProjectile);
			}
		}
	}
	else if (mAnimationList[M rightAttack2]->GetIsPlay() or mAnimationList[M leftAttack2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			if (mCurrentAnimation->GetCurrentFrameIndex() == 1 or mCurrentAnimation->GetCurrentFrameIndex() == 3)
			{
				new Bullet(mWarlockProjectile, "WarlockProjectile", this, mMagicalAttackPower, 400, 600, mAngle, BulletType::FrameProjectile);
			}
		}
	}
}

void HighWarlock::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) mSkill1CoolTime = 0;
}

void HighWarlock::Skill2()
{
	mSkill2CoolTime -= dTime;
	if (mSkill2CoolTime < 0) mSkill2CoolTime = 0;

	if (mAnimationList[M rightSkill2]->GetIsPlay() or mAnimationList[M leftSkill2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			if (mAnimationList[M rightSkill2]->GetNowFrameX() == 1 or mAnimationList[M leftSkill2]->GetNowFrameX() == 6)
			{
				Dash(5);
				CAMERA->PanningOn(5);
			}

			if (mAnimationList[M rightSkill2]->GetNowFrameX() > 0 and mAnimationList[M rightSkill2]->GetNowFrameX() < 5)
			{
				Attack(1, 2, AttackType::Whirlwind); //애들 모으는 함수가 필요해
			}
			else if (mAnimationList[M leftSkill2]->GetNowFrameX() < 7 and mAnimationList[M leftSkill2]->GetNowFrameX() > 2)
			{
				Attack(1, 2, AttackType::Whirlwind); //애들 모으는 함수가 필요해
			}

			if (mAnimationList[M rightSkill2]->GetNowFrameX() == 5 or mAnimationList[M leftSkill2]->GetNowFrameX() == 2)
			{
				Attack(1, 1, AttackType::Side, true);
				Attack(2, 3, AttackType::Side);
				CAMERA->PanningOn(5);
			}
		}
	}
}

void HighWarlock::SkulSwitch(int indexX, int indexY)
{
	Player::SkulSwitch(indexX, indexY);
	new Bullet(mWarlockBarricade, "Barricade", this, mMagicalAttackPower, 0, 1, 0, BulletType::Barricade);
}

void HighWarlock::SkulReset()
{
	mCurrentAnimation->Stop();
}

void HighWarlock::SetAttackSpeed()
{
	mAnimationList[M rightAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M rightAttack2]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack2]->SetFrameUpdateTime(mAttackSpeed);
}
