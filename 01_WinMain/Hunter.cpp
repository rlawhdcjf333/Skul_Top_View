#include "pch.h"
#include "Hunter.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "Animation.h"

Hunter::Hunter(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"Hunter", Resources(L"/skul/skul_hunter.bmp"), 700, 1000, 7, 10, true);
	mImage = IMAGEMANAGER->FindImage(L"Hunter");

	IMAGEMANAGER->LoadFromFile(L"Arrow", Resources(L"/skul/arrow1.bmp"), 210, 100, 1, 2, true);
	mArrow = IMAGEMANAGER->FindImage(L"Arrow");

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

	mAnimationList[M rightAttack1] = new Animation(0, 6, 6, 6, false, false, (float)mAttackSpeed/3);
	mAnimationList[M leftAttack1] = new Animation(0, 7, 6, 7, true, false, (float)mAttackSpeed/3);

	mAnimationList[M rightSkill1] = new Animation(0, 8, 6, 8, false, false, 0.03f);
	mAnimationList[M leftSkill1] = new Animation(0, 9, 6, 9, true, false, 0.03f);
	mAnimationList[M rightSkill2] = new Animation(0, 8, 6, 8, false, false, 0.03f);
	mAnimationList[M leftSkill2] = new Animation(0, 9, 6, 9, true, false, 0.03f);

	mAnimationList[M leftSwitching] = new Animation(0, 4, 6, 4, false, false, 0.03f,
		[this]() { SetAnimation(M leftSkill2);});
	mAnimationList[M rightSwitching] = new Animation(0, 5, 6, 5, true, false, 0.03f,
		[this]() {SetAnimation(M rightSkill2);});

	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
}

void Hunter::Update()
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
	if (mDashCoolTime < 0) { mDashCoolTime = 0; mDashCount = 0; }

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

	if (INPUT->GetKey('X'))
	{
		mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
		if (RIGHT) { SetAnimation(M rightAttack1); }
		if (LEFT) { SetAnimation(M leftAttack1); }
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

void Hunter::SetAnimation(int listNum)
{
	if (mAnimationList[M rightDash]->GetIsPlay()) return;
	if (mAnimationList[M leftDash]->GetIsPlay()) return;

	if (mAnimationList[M rightAttack1]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack1]->GetIsPlay()) return;

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
	if (mAnimationList[M rightAttack1]->GetIsPlay() or mAnimationList[M leftAttack1]->GetIsPlay())
	{
		if (mCurrentAnimation->GetNowFrameX() == 3 and mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
			new Bullet(mArrow, "Bullet", this, 1*mPhysicalAttackPower, 300, 500, mAngle, BulletType::Straight);
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

		if (mCurrentAnimation->GetCurrentFrameTime() < dTime and mCurrentAnimation->GetCurrentFrameIndex() == 3)
		{
			mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());

			for (int i = 0; i < 5; i++)
				new Bullet(mArrow, "Bullet", this, 3 * mPhysicalAttackPower, 300, 500, mAngle - PI / 3 + i * PI / 6, BulletType::Straight);

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

		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			if (mCurrentAnimation->GetNowFrameX() == 3)
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				new Bullet(mArrow, "Bullet", this, 3*mPhysicalAttackPower, 600, 1000, mAngle, BulletType::Piercing);
				CAMERA->PanningOn(5);
			}
		}
	}
}

void Hunter::SkulSwitch(int indexX, int indexY)
{
	Player::SkulSwitch(indexX, indexY);
	if (LEFT)
	{
		Dash(5, true);
		SetAnimation(M leftSwitching);
	}
	if (RIGHT)
	{
		Dash(5, true);
		SetAnimation(M rightSwitching);
	}
}

void Hunter::SkulReset()
{
	mCurrentAnimation->Stop();
}

void Hunter::SetAttackSpeed()
{
	mAnimationList[M rightAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack1]->SetFrameUpdateTime(mAttackSpeed);
}
