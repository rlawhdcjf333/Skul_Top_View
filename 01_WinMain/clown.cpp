#include "pch.h"
#include "Clown.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "Animation.h"
#include "ClownDagger.h"
#include "ClownBox.h"

Clown::Clown(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"Clown", Resources(L"/skul/skul_clown.bmp"), 800, 1600, 8, 16, true);
	mImage = IMAGEMANAGER->FindImage(L"Clown");

	IMAGEMANAGER->LoadFromFile(L"ClownMark", Resources(L"/skul/clownMark.bmp"), 1300, 100, 13, 1, true);

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

	mAnimationList[M rightAttack1] = new Animation(0, 6, 4, 6, false, false, mAttackSpeed);
	mAnimationList[M leftAttack1] = new Animation(0, 7, 4, 7, true, false, mAttackSpeed);

	mAnimationList[M rightSkill1] = new Animation(0, 10, 5, 10, false, false, 0.1f);
	mAnimationList[M leftSkill1] = new Animation(0, 11, 5, 11, true, false, 0.1f);
	mAnimationList[M rightSkill2] = new Animation(0, 12, 5, 12, false, false, 0.1f);
	mAnimationList[M leftSkill2] = new Animation(0, 13, 5, 13, true, false, 0.1f);

	mAnimationList[M rightSwitching] = new Animation(0, 14, 6, 14, false, false, 0.1f,
		[this]() {
			UpdateAngle();
			for (int i = 0; i < 4; i++) {
				new ClownDagger(this, 2*mPhysicalAttackPower, mAngle-PI/6+i*PI/12, 500); //여기에도 표식 활성화
			}
		});
	mAnimationList[M leftSwitching] = new Animation(0, 15, 6, 15, true, false, 0.1f,
		[this]() {
			UpdateAngle();
			for (int i = 0; i < 4; i++) {
				new ClownDagger(this, 2*mPhysicalAttackPower, mAngle - PI / 6 + i * PI / 12, 500);
			}
		});

	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
}

void Clown::Update()
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
	if (mDashCoolTime < 0) { mDashCoolTime = 0, mDashCount = 0; }

	if (INPUT->GetKeyDown('Z')) //대쉬
	{
		if (mDashCoolTime == 0)
		{
			mCurrentAnimation->Stop();
			Dash(5); //팡머 특; 연속대쉬 안되는 대신 조금 더 멀리 구름
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
		UpdateAngle();
		if (RIGHT) { SetAnimation(M rightAttack1); }
		if (LEFT) { SetAnimation(M leftAttack1); }
	}
	BasicAttack();

	if (INPUT->GetKeyDown('A')) // 폭발성 단검
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

	if (INPUT->GetKeyDown('S')) // 폭발성 상자
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
		if (mCurrentAnimation->GetNowFrameX() == 2 and mCurrentAnimation->GetCurrentFrameTime() > mAttackSpeed-dTime)
		{
			new ClownDagger(this, 2*mPhysicalAttackPower, mAngle, 500); // 광대 단검 자체 패시브 -3타 표식에 폭발 효과
		}
	}
}

void Clown::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) mSkill1CoolTime = 0;

	if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
	{
		mSkill1CoolTime = 12;

		if (mCurrentAnimation->GetCurrentFrameTime() > 0.1f-dTime and mCurrentAnimation->GetNowFrameX() == 2)
		{
			for (int i = 0; i < 3; i++)
				new ClownDagger(this, 3 * mMagicalAttackPower, mAngle - PI / 12 + i * PI / 24, 500, true); //여기에도 표식이 묻음

			CAMERA->PanningOn(5);
		}
	}
}

void Clown::Skill2()
{
	mSkill2CoolTime -= dTime;
	if (mSkill2CoolTime < 0) mSkill2CoolTime = 0;

	if (mAnimationList[M rightSkill2]->GetIsPlay() or mAnimationList[M leftSkill2]->GetIsPlay())
	{
		mSkill2CoolTime = 14;

		if (mCurrentAnimation->GetCurrentFrameTime() > 0.1f-dTime)
		{
			
			switch (mCurrentAnimation->GetCurrentFrameIndex())
			{
				case 1:
				case 2:
				case 3:
				case 4:
					Dash(1);
					new ClownBox(this, 3 * mMagicalAttackPower, 0);
					break;
				default:
					break;
			}
			
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

void Clown::SetAttackSpeed()
{
	mAnimationList[M rightAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack1]->SetFrameUpdateTime(mAttackSpeed);
}
