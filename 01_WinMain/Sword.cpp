#include "pch.h"
#include "Sword.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "Animation.h"

Sword::Sword(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"Sword", Resources(L"/skul/skul_sword.bmp"), 1200,1500, 12,15, true);
	mImage = IMAGEMANAGER->FindImage(L"Sword");

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}

void Sword::Init()
{
	mTileSelect = new TileSelect;

	mAnimationList[M rightIdle] = new Animation(0, 0, 3, 0, false, true, 0.2f);
	mAnimationList[M leftIdle] = new Animation(0, 1, 3, 1, false, true, 0.2f);
	mAnimationList[M rightWalk] = new Animation(0, 2, 7, 2, false, true, 0.2f);
	mAnimationList[M leftWalk] = new Animation(0, 3, 7, 3, false, true, 0.2f);
	mAnimationList[M rightDash] = new Animation(0, 4, 0, 4, false, false, 0.35f);
	mAnimationList[M leftDash] = new Animation(1, 4, 1, 4, false, false, 0.35f);
	
	mAnimationList[M rightAttack1] = new Animation(0, 5, 4, 5, false, false, 0.1f,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M rightAttack2] = new Animation(0, 7, 4, 7, false, false, 0.1f,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack3);
				if (LEFT) SetAnimation(M leftAttack3);
			}
		});
	mAnimationList[M rightAttack3] = new Animation(0, 9, 5, 9, false, false, 0.1f);
	mAnimationList[M leftAttack1] = new Animation(0, 6, 4, 6, false, false, 0.1f,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M leftAttack2] = new Animation(0, 8, 4, 8, false, false, 0.1f,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack3);
				if (LEFT) SetAnimation(M leftAttack3);
			}
		});
	mAnimationList[M leftAttack3] = new Animation(0, 10, 5, 10, false, false, 0.1f);


	mAnimationList[M rightSwitching] = new Animation(2, 4, 2, 4, false, false, 0.5f);
	mAnimationList[M leftSwitching] = new Animation(3, 4, 3, 4, false, false, 0.5f);

	mAnimationList[M rightSkill1] = new Animation(0, 11, 3, 11, false, false, 0.1f);
	mAnimationList[M leftSkill1] = new Animation(0, 12, 3, 12, false, false, 0.1f);

	mAnimationList[M rightSkill2] = new Animation(0, 13, 11, 13, false, false, 0.1f);
	mAnimationList[M leftSkill2] = new Animation(0, 14, 11, 14, false, false, 0.1f);

	SetAnimation(M rightIdle);
}

void Sword::Update()
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

	if (INPUT->GetKeyDown(VK_SPACE))
	{
		if (RIGHT) { SetAnimation(M rightSwitching); }
		if (LEFT) { SetAnimation(M leftSwitching); }
	}

	if (INPUT->GetKey('X'))
	{
		mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
		if (RIGHT) { SetAnimation(M rightAttack1); }
		if (LEFT) { SetAnimation(M leftAttack1); }
	}
	BasicAttack();

	if (INPUT->GetKeyDown('A'))
	{
		mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
		if (RIGHT) { SetAnimation(M rightSkill1); }
		if (LEFT) { SetAnimation(M leftSkill1); }
	}
	Skill1();

	if (INPUT->GetKeyDown('S'))
	{
		mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
		if (RIGHT) { SetAnimation(M rightSkill2); }
		if (LEFT) { SetAnimation(M leftSkill2); }
	}
	Skill2();

	if (INPUT->GetKeyDown('Z')) //대쉬
	{
		Dash(5);
		if (LEFT) SetAnimation(M leftDash);
		if (RIGHT) SetAnimation(M rightDash);
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

	mCurrentAnimation->Update();

	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}

void Sword::Release()
{
	SafeDelete(mTileSelect);

	for (Animation* elem : mAnimationList)
	{
		SafeDelete(elem);
	}
}

void Sword::Render(HDC hdc)
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

void Sword::SetAnimation(int listNum)
{
	if (mAnimationList[M rightAttack1]->GetIsPlay()) return;
	if (mAnimationList[M rightAttack2]->GetIsPlay()) return;
	if (mAnimationList[M rightAttack3]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack1]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack2]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack3]->GetIsPlay()) return;

	if (mAnimationList[M rightDash]->GetIsPlay()) return;
	if (mAnimationList[M leftDash]->GetIsPlay()) return;
	if (mAnimationList[M rightSwitching]->GetIsPlay()) return;
	if (mAnimationList[M leftSwitching]->GetIsPlay()) return;

	if (mAnimationList[M rightSkill1]->GetIsPlay()) return;
	if (mAnimationList[M rightSkill2]->GetIsPlay()) return;
	if (mAnimationList[M leftSkill1]->GetIsPlay()) return;
	if (mAnimationList[M leftSkill2]->GetIsPlay()) return;

	mCurrentAnimation = mAnimationList[listNum];
	mCurrentAnimation->Play();
}

void Sword::BasicAttack()
{
	if (mAnimationList[M rightAttack1]->GetIsPlay() or mAnimationList[M rightAttack2]->GetIsPlay()
		or mAnimationList[M leftAttack1]->GetIsPlay() or mAnimationList[M leftAttack2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetNowFrameX() == 2 and mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			Attack(1, 1, AttackType::Side); 
		}
	}
	else if (mAnimationList[M rightAttack3]->GetIsPlay() or mAnimationList[M leftAttack3]->GetIsPlay())
	{
		if (mCurrentAnimation->GetNowFrameX() == 2 and mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			Attack(1, 2, AttackType::Stab); 
			Dash(2);
		}
	}
}

void Sword::Skill1()
{
	if ((mAnimationList[M rightSkill1]->GetCurrentFrameTime() < dTime and mAnimationList[M rightSkill1]->GetNowFrameX() == 1)
		or (mAnimationList[M leftSkill1]->GetCurrentFrameTime() < dTime and mAnimationList[M leftSkill1]->GetNowFrameX() == 1))
	{
		Attack(1, 3, AttackType::Stab);
		Dash(3);
		CAMERA->PanningOn(5);
	}
	
}

void Sword::Skill2()
{
	if (mAnimationList[M rightSkill2]->GetIsPlay() or mAnimationList[M leftSkill2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			if (mCurrentAnimation->GetNowFrameX() == 2 or mCurrentAnimation->GetNowFrameX() == 5 or mCurrentAnimation->GetNowFrameX() == 8)
			{
				Attack(1, 3, AttackType::Stab);
				Dash(3);
				CAMERA->PanningOn(5);
			}
		}
	}
}
