#include "pch.h"
#include "Mino.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "Animation.h"
#include "Enemy.h"
#include "Effect.h"

Mino::Mino(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"Mino", Resources(L"/skul/skul_mino.bmp"), 800, 1100, 8, 11, true);
	mImage = IMAGEMANAGER->FindImage(L"Mino");

	IMAGEMANAGER->LoadFromFile(L"MinoBlunt", Resources(L"/skul/minoBlunt.bmp"), 1100, 100, 11, 1, true);
	IMAGEMANAGER->LoadFromFile(L"MinoBomb", Resources(L"/skul/minoBomb.bmp"), 400, 300, 4, 3, true);

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
	mAnimationList[M rightSkill2] = new Animation(0, 5, 5, 5, false, false, 0.1f);
	mAnimationList[M rightSkill2CancelAttack] = new Animation(0, 7, 4, 7, false, false, 0.1f);
	mAnimationList[M leftSkill2] = new Animation(0, 6, 5, 6, true, false, 0.1f);
	mAnimationList[M leftSkill2CancelAttack] = new Animation(0, 8, 4, 8, true, false, 0.1f);

	mAnimationList[M rightSwitching] = new Animation(0,5,5,5,false,false,0.1f);
	mAnimationList[M leftSwitching] = new Animation(0,6,5,6,true, false, 0.1f);

	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
	mPassiveDuration = 0;
}

void Mino::Update()
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
	if (mDashCoolTime < 0) mDashCoolTime = 0;
	if (INPUT->GetKeyDown('Z')) //대쉬
	{
		if (mDashCoolTime == 0)
		{
			mCurrentAnimation->Stop();
			Dash(3);
			Attack(1, 4, AttackType::Stab);
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
		if (!mAnimationList[M rightAttack1]->GetIsPlay() and !mAnimationList[M rightAttack2]->GetIsPlay()
			and !mAnimationList[M leftAttack1]->GetIsPlay() and !mAnimationList[M leftAttack2]->GetIsPlay())
		{
			UpdateAngle();
		}
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

	Passive();

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
	
	if (mAnimationList[M rightSkill2CancelAttack]->GetIsPlay()) return;
	if (mAnimationList[M leftSkill2CancelAttack]->GetIsPlay()) return;


	if (mAnimationList[M rightSwitching]->GetIsPlay()) return;
	if (mAnimationList[M leftSwitching]->GetIsPlay()) return;

	mCurrentAnimation = mAnimationList[listNum];
	mCurrentAnimation->Play();
}

void Mino::BasicAttack()
{
	if (mAnimationList[M rightAttack1]->GetIsPlay() or mAnimationList[M leftAttack1]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() > mAttackSpeed-dTime)
		{
			if (mCurrentAnimation->GetCurrentFrameIndex() ==3)
			{
				Attack(mPhysicalAttackPower, 2, AttackType::Side);
			}
		}
	}
	else if (mAnimationList[M rightAttack2]->GetIsPlay() or mAnimationList[M leftAttack2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetNowFrameX() == 2 and mCurrentAnimation->GetCurrentFrameTime() > mAttackSpeed-dTime)
		{
			Attack(mPhysicalAttackPower, 2, AttackType::Side);
		}
	}
}

void Mino::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) mSkill1CoolTime = 0;

	if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
	{
		mPassiveDuration = 3;
		mSkill1CoolTime = 10;
		if (mCurrentAnimation->GetCurrentFrameTime() > 0.05f-dTime)
		{
			switch (mCurrentAnimation->GetCurrentFrameIndex())
			{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
					Attack(2*mPhysicalAttackPower, 1, AttackType::Side);
					Dash(1);
					CAMERA->PanningOn(5);
					break;
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
		mPassiveDuration = 3;
		mSkill2CoolTime = 14;
		if (mCurrentAnimation->GetCurrentFrameIndex() > 0)
		{
			if (INPUT->GetKeyDown('S'))
			{
				mCurrentAnimation->Stop();
				if (RIGHT) mCurrentAnimation = mAnimationList[M rightSkill2CancelAttack];
				if (LEFT) mCurrentAnimation = mAnimationList[M leftSkill2CancelAttack];
				mCurrentAnimation->Play();
				Attack(mPhysicalAttackPower, 2, AttackType::Whirlwind);
				(new Effect(L"MinoBlunt", mX, mY, EffectType::Normal))->Scaling(200,200);
				for (auto elem : Obj->GetObjectList(ObjectLayer::Enemy))
				{
					Enemy* downcast = (Enemy*)elem;
					if (downcast->GetHitTime() == 0.6f) //방금 공격에 맞은 친구들만 대상으로 스턴
					{
						downcast->Stun(3); //스턴은 별거 없고 그냥 힛타임을 3초로 강제로 늘림 -->에너미에서 힛타임이 0.6 이하일때만 갱신되게 조정
					}
				}

				CAMERA->PanningOn(7);
			}
		}

		if (mCurrentAnimation->GetCurrentFrameTime() > 0.1f - dTime)
		{
			switch (mCurrentAnimation->GetCurrentFrameIndex())
			{
			default:
				Attack(2*mPhysicalAttackPower, 1, AttackType::Side);
				Dash(1);
				CAMERA->PanningOn(3);
			}
		}
	}
}

void Mino::SkulSwitch(int indexX, int indexY)
{
	Player::SkulSwitch(indexX, indexY);
	if (LEFT)
	{
		Attack(2*mPhysicalAttackPower, 2, AttackType::Side);
		SetAnimation(M leftSwitching);
	}
	if (RIGHT)
	{
		Attack(2*mPhysicalAttackPower, 2, AttackType::Side);
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

void Mino::Passive()
{
	mPassiveDuration -= dTime;
	if (mPassiveDuration < 0) mPassiveDuration = 0;

	for(int i=0; i<6; i++)
	if (mPassiveDuration > 0.4f + (float)i/2 and mPassiveDuration<= 0.4f + dTime + (float)i/2)
	{
		(new Effect(L"MinoBomb", mX, mY-30, EffectType::Normal))->Scaling(200, 200);
		Attack(mPhysicalAttackPower, 2, AttackType::Whirlwind);
	}
}