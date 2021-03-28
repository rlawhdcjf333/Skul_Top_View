#include "pch.h"
#include "Sword.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "Animation.h"
#include "Enemy.h"
#include "Bleeding.h"
#include "TetanusBuff.h"

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
	
	mAnimationList[M rightAttack1] = new Animation(0, 5, 4, 5, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M rightAttack2] = new Animation(0, 7, 4, 7, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack3);
				if (LEFT) SetAnimation(M leftAttack3);
			}
		});
	mAnimationList[M rightAttack3] = new Animation(0, 9, 5, 9, false, false, mAttackSpeed);
	mAnimationList[M leftAttack1] = new Animation(0, 6, 4, 6, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M leftAttack2] = new Animation(0, 8, 4, 8, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack3);
				if (LEFT) SetAnimation(M leftAttack3);
			}
		});
	mAnimationList[M leftAttack3] = new Animation(0, 10, 5, 10, false, false, mAttackSpeed);

	mAnimationList[M rightSwitching] = new Animation(2, 4, 2, 4, false, false, 0.5f);
	mAnimationList[M leftSwitching] = new Animation(3, 4, 3, 4, false, false, 0.5f);

	mAnimationList[M rightSkill1] = new Animation(0, 11, 3, 11, false, false, 0.1f);
	mAnimationList[M leftSkill1] = new Animation(0, 12, 3, 12, false, false, 0.1f);

	mAnimationList[M rightSkill2] = new Animation(0, 13, 11, 13, false, false, 0.1f);
	mAnimationList[M leftSkill2] = new Animation(0, 14, 11, 14, false, false, 0.1f);

	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
}

void Sword::Update()
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

	if (INPUT->GetKey('X')) //3타 연계 기본공격
	{
		if (!mAnimationList[M rightAttack1]->GetIsPlay() and !mAnimationList[M rightAttack2]->GetIsPlay()
			and !mAnimationList[M leftAttack1]->GetIsPlay() and !mAnimationList[M leftAttack2]->GetIsPlay()
			and !mAnimationList[M rightAttack3]->GetIsPlay() and !mAnimationList[M leftAttack3]->GetIsPlay())
		{
			UpdateAngle();
		}
		if (RIGHT) { SetAnimation(M rightAttack1); }
		if (LEFT) { SetAnimation(M leftAttack1); }
	}
	BasicAttack();

	if (INPUT->GetKeyDown('A')) // 찌르기 돌진
	{
		if(mSkill1CoolTime==0)
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

	if (INPUT->GetKeyDown('S')) // 세번 찌르기
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

	mCurrentAnimation->Update();

	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	mHitBox = RectMakeBottom(mX, mY, 30, 30);
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
		if (mCurrentAnimation->GetNowFrameX() == 2 and mCurrentAnimation->GetCurrentFrameTime() > mAttackSpeed- dTime)
		{
			Attack(mPhysicalAttackPower, 1, AttackType::Side); 

			for (GameObject* elem : Obj->GetObjectList(ObjectLayer::Enemy))
			{
				Enemy* downcast = (Enemy*)elem;
				if (downcast->GetHitTime() == 0.6f and RAND->Probablity(5)) //5% 확률 출혈 부여 패시브. 최대 힛타임 0.6에서만 동작
				{
					new Bleeding(elem, mMagicalAttackPower, 3);
				}
			}

		}
	}
	else if (mAnimationList[M rightAttack3]->GetIsPlay() or mAnimationList[M leftAttack3]->GetIsPlay())
	{
		if (mCurrentAnimation->GetNowFrameX() == 2 and mCurrentAnimation->GetCurrentFrameTime() > mAttackSpeed- dTime)
		{
			Dash(1);
			Attack(mPhysicalAttackPower, 2, AttackType::Stab);
			for (GameObject* elem : Obj->GetObjectList(ObjectLayer::Enemy))
			{
				Enemy* downcast = (Enemy*)elem;
				if (downcast->GetHitTime() == 0.6f and RAND->Probablity(5)) //5% 확률 출혈 부여
				{
					new Bleeding(elem, mMagicalAttackPower, 3);
				}
			}
		}
	}
}

void Sword::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) mSkill1CoolTime = 0;

	if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
	{
		mSkill1CoolTime = 11;

		if (mCurrentAnimation->GetCurrentFrameTime() > 0.1f- dTime and mCurrentAnimation->GetCurrentFrameIndex() == 1)
		{
			Dash(5);
			Attack(2*mPhysicalAttackPower, 5, AttackType::Stab);
			CAMERA->PanningOn(5);
		}
	}
}

void Sword::Skill2()
{
	mSkill2CoolTime -= dTime;
	if (mSkill2CoolTime < 0) mSkill2CoolTime = 0;

	if (mAnimationList[M rightSkill2]->GetIsPlay() or mAnimationList[M leftSkill2]->GetIsPlay())
	{
		mSkill2CoolTime = 6;

		if (mCurrentAnimation->GetCurrentFrameTime() > 0.1f-dTime)
		{
			if (mCurrentAnimation->GetNowFrameX() == 2 or mCurrentAnimation->GetNowFrameX() == 5 or mCurrentAnimation->GetNowFrameX() == 8)
			{
				Dash(3);
				Attack(2*mPhysicalAttackPower, 3, AttackType::Stab);
				CAMERA->PanningOn(5);
			}
		}
	}
}

void Sword::SkulSwitch(int indexX, int indexY)
{
	Player::SkulSwitch(indexX, indexY);
	if (LEFT) SetAnimation(M leftSwitching);
	if (RIGHT) SetAnimation(M rightSwitching);
	
	new TetanusBuff(this, 6); //교대 시 6초간 모든 공격에 출혈 발생
}

void Sword::SkulReset() {
	mCurrentAnimation->Stop();
}

void Sword::SetAttackSpeed()
{
	mAnimationList[M rightAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M rightAttack2]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M rightAttack3]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack2]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack3]->SetFrameUpdateTime(mAttackSpeed);
}