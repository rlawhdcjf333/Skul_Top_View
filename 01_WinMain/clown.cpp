#include "pch.h"
#include "Clown.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "Animation.h"

Clown::Clown(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"Clown", Resources(L"/skul/skul_clown.bmp"), 800, 1600, 8, 16, true);
	mImage = IMAGEMANAGER->FindImage(L"Clown");

	IMAGEMANAGER->LoadFromFile(L"Dagger", Resources(L"/skul/dagger.bmp"), 15, 14, 1, 2, true);
	mDagger = IMAGEMANAGER->FindImage(L"Dagger");

	IMAGEMANAGER->LoadFromFile(L"Box", Resources(L"/skul/box.bmp"), 30, 47, true);
	mBox = IMAGEMANAGER->FindImage(L"Box");

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}
void Clown::Init()
{
	mTileSelect = new TileSelect;

	mAnimationList[M rightIdle] = new Animation(0, 0, 7, 0, false, true, 0.1f);
	mAnimationList[M leftIdle] = new Animation(0, 1, 7, 1, true, true, 0.1f);
	mAnimationList[M rightWalk] = new Animation(0, 2, 7, 2, false, true, 0.1f);
	mAnimationList[M leftWalk] = new Animation(0, 3, 7, 3, true, true, 0.1f);

	mAnimationList[M rightDash] = new Animation(0, 4, 6, 4, false, false, 0.05f);
	mAnimationList[M leftDash] = new Animation(0, 5, 6, 5, true, false, 0.05f);

	mAnimationList[M rightAttack1] = new Animation(0, 6, 4, 6, false, false, 0.05f);
	mAnimationList[M leftAttack1] = new Animation(0, 7, 4, 7, true, false, 0.05f);

	mAnimationList[M rightSkill1] = new Animation(0, 10, 5, 10, false, false, 0.05f);
	mAnimationList[M leftSkill1] = new Animation(0, 11, 5, 11, true, false, 0.05f);
	mAnimationList[M rightSkill2] = new Animation(0, 12, 5, 12, false, false, 0.05f);
	mAnimationList[M leftSkill2] = new Animation(0, 13, 5, 13, true, false, 0.05f);

	mAnimationList[M rightSwitching] = new Animation(0, 14, 6, 14, false, false, 0.05,
		[this]() {
			mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
			for (int i = 0; i < 4; i++) {
				new Bullet(mDagger, "Daggers", this, 1, 500, 500, mAngle+PI/6-i*PI/12, BulletType::Straight);
			}
		});
	mAnimationList[M leftSwitching] = new Animation(0, 15, 6, 15, true, false, 0.05f,
		[this]() {
			mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
			for (int i = 0; i < 4; i++) {
				new Bullet(mDagger, "Daggers", this, 1, 500, 500, mAngle + PI / 6 - i * PI / 12, BulletType::Straight);
			}
		});

	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
}

void Clown::Update()
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
			if (LEFT) SetAnimation(M leftDash);
			if (RIGHT) SetAnimation(M rightDash);
			mDashCount = 1;
			mDashCoolTime = mInitDashCoolTime;
		}
		else if (mAnimationList[M leftDash]->GetIsPlay() or mAnimationList[M rightDash]->GetIsPlay())
		{

			if (mDashCount == 1)
			{
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

	if (INPUT->GetKeyDown('A')) // 폭발성 단검
	{
		if (mSkill1CoolTime == 0)
		{
			mSkill1CoolTime = 12;
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

	if (INPUT->GetKeyDown('S')) // 폭발성 상자
	{
		if (mSkill2CoolTime == 0)
		{
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


}

void Clown::Release()
{
	SafeDelete(mTileSelect);

	for (Animation* elem : mAnimationList)
	{
		SafeDelete(elem);
	}
}

void Clown::Render(HDC hdc)
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

void Clown::SetAnimation(int listNum)
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

void Clown::BasicAttack()
{
	if (mAnimationList[M rightAttack1]->GetIsPlay() or mAnimationList[M leftAttack1]->GetIsPlay())
	{
		if (mCurrentAnimation->GetNowFrameX() == 2 and mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
			new Bullet(mDagger, "Bullet", this, 3, 500, 500, mAngle, BulletType::Straight);
		}
	}
}

void Clown::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) mSkill1CoolTime = 0;

	if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime and mCurrentAnimation->GetNowFrameX() == 2)
		{
			mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
			for(int i=0; i<2; i++)
			new Bullet(mDagger, "ExplosiveDagger", this, 2, 500, 500, mAngle+PI/12-i*PI/6, BulletType::Flask);

			CAMERA->PanningOn(5);

		}
}

void Clown::Skill2()
{
	mSkill2CoolTime -= dTime;
	if (mSkill2CoolTime < 0) mSkill2CoolTime = 0;

	if (mAnimationList[M rightSkill2]->GetIsPlay() or mAnimationList[M leftSkill2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			if (mAnimationList[M rightSkill2]->GetNowFrameX() == 0 or mAnimationList[M leftSkill2]->GetNowFrameX() == 5)
			{
				Dash(5);
				CAMERA->PanningOn(5);
			}
			
			mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
			new Bullet(mBox, "Box", this, 3, 60, 30, PI/2, BulletType::Flask);
			
			
		}
	}
}

void Clown::SkulSwitch(int indexX, int indexY)
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

void Clown::SkulReset()
{
	mCurrentAnimation->Stop();
}
