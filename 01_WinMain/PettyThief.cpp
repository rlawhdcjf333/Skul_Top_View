#include "pch.h"
#include "PettyThief.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "Animation.h"
#include "Effect.h"
#include "Enemy.h"

PettyThief::PettyThief(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"PettyThief", Resources(L"/skul/skul_petty_thief.bmp"), 700, 1600, 7, 16, true);
	mImage = IMAGEMANAGER->FindImage(L"PettyThief");

	IMAGEMANAGER->LoadFromFile(L"ThiefSmoke", Resources(L"/skul/thiefSmoke.bmp"),600,600,6,6, true);

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}
void PettyThief::Init()
{
	mTileSelect = new TileSelect;

	mAnimationList[M rightIdle] = new Animation(0, 0, 3, 0, false, true, 0.2f);
	mAnimationList[M leftIdle] = new Animation(0, 1, 3, 1, false, true, 0.2f);
	mAnimationList[M rightWalk] = new Animation(0, 2, 5, 2, false, true, 0.2f);
	mAnimationList[M leftWalk] = new Animation(0, 3, 5, 3, false, true, 0.2f);

	mAnimationList[M rightDash] = new Animation(0, 4, 6, 4, false, false, 0.05f);
	mAnimationList[M leftDash] = new Animation(0, 5, 6, 5, false, false, 0.05f);

	mAnimationList[M rightAttack1] = new Animation(0, 6, 4, 6, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M rightAttack2] = new Animation(0, 8, 4, 8, false, false, mAttackSpeed);
	mAnimationList[M leftAttack1] = new Animation(0, 7, 4, 7, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M leftAttack2] = new Animation(0, 9, 4, 9, false, false, mAttackSpeed);

	mAnimationList[M rightSkill1] = new Animation(0, 10, 4, 10, false, false, 0.1f);
	mAnimationList[M leftSkill1] = new Animation(0, 11, 4, 11, false, false, 0.1f);
	mAnimationList[M rightSkill2] = new Animation(0, 12, 7, 12, false, false, 0.1f);
	mAnimationList[M leftSkill2] = new Animation(0, 13, 7, 13, false, false, 0.1f);

	mAnimationList[M rightSwitching] = new Animation(0, 4, 7, 4, true, false, 0.1f,
		[this]() {
			Attack(mPhysicalAttackPower, 1, AttackType::Side);
		});
	mAnimationList[M leftSwitching] = new Animation(0, 5, 7, 5, true, false, 0.1f,
		[this]() {
		Attack(mPhysicalAttackPower, 1, AttackType::Side);
		});

	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
	mSmokeCount = 3;
}

void PettyThief::Update()
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
			SKUL->Disinvincibilize();
			mCurrentAnimation->Stop();
			Dash(4); //좀도둑의 패시브 효과로 대쉬거리가 1 높다.
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
				Dash(4);
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
		if (!mAnimationList[M rightAttack1]->GetIsPlay() and !mAnimationList[M rightAttack2]->GetIsPlay()
			and !mAnimationList[M leftAttack1]->GetIsPlay() and !mAnimationList[M leftAttack2]->GetIsPlay())
		{
			UpdateAngle();
		}
		if (RIGHT) { SetAnimation(M rightAttack1); }
		if (LEFT) { SetAnimation(M leftAttack1); }
	}
	BasicAttack();

	if (INPUT->GetKeyDown('A')) // 주머니 찢기
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

	if (INPUT->GetKeyDown('S')) // 연막치고 뒤구르기
	{
		if (mSmokeCount>0)
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

void PettyThief::Release()
{
	SafeDelete(mTileSelect);

	for (Animation* elem : mAnimationList)
	{
		SafeDelete(elem);
	}
}

void PettyThief::Render(HDC hdc)
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

void PettyThief::SetAnimation(int listNum)
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

	if (mAnimationList[M rightSwitching]->GetIsPlay()) return;
	if (mAnimationList[M leftSwitching]->GetIsPlay()) return;

	mCurrentAnimation = mAnimationList[listNum];
	mCurrentAnimation->Play();
}

void PettyThief::BasicAttack()
{
	if (mAnimationList[M rightAttack1]->GetIsPlay() or mAnimationList[M leftAttack1]->GetIsPlay()
		or mAnimationList[M rightAttack2]->GetIsPlay() or mAnimationList[M leftAttack2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetNowFrameX() == 2 and mCurrentAnimation->GetCurrentFrameTime() > mAttackSpeed-dTime)
		{
			Attack(mPhysicalAttackPower, 1, AttackType::Side);
			auto tmp = Obj->GetObjectList(ObjectLayer::Enemy); //10% 확률로 삥뜯는 패시브 로직
			for (GameObject* elem : tmp)
			{
				Enemy* downcast = (Enemy*)elem;
				if (downcast->GetHitTime() == 0.6f and RAND->Probablity(10)) //적이 공격에 피격된 순간에는 최대 힛타임. 힛타임이 에너미마다 서로 다르다면 maxhitTime으로 변수화해야함
				{
					SKUL->PlusGold(3);
					(new Effect(L"GoldGet", downcast->GetRect().left, downcast->GetRect().top - 15, EffectType::Normal))->Scaling(50, 50);
				}
			}
		}
	}
}

void PettyThief::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) mSkill1CoolTime = 0;

	if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
	{
		mSkill1CoolTime = 8;

		if (mCurrentAnimation->GetCurrentFrameTime() >0.1f- dTime)
		{
			switch (mCurrentAnimation->GetCurrentFrameIndex())
			{
			case0: 
				SKUL->Invincibilize();
				break;
			case 2:
				Attack(2*mPhysicalAttackPower, 2, AttackType::Whirlwind);
				CAMERA->PanningOn(5);
				auto tmp = Obj->GetObjectList(ObjectLayer::Enemy);
				for (GameObject* elem : tmp)
				{
					Enemy* downcast = (Enemy*)elem;
					if (downcast->GetHitTime() > 0)
					{
						SKUL->PlusGold(2);
						(new Effect(L"GoldGet", downcast->GetRect().left, downcast->GetRect().top-15, EffectType::Normal))->Scaling(50,50);
					}
				}
				break;
			case4:

				SKUL->Disinvincibilize();
				break;
			}
		}
		
	}
}

void PettyThief::Skill2()
{
	mSkill2CoolTime -= dTime;
	if (mSkill2CoolTime < 0) { mSkill2CoolTime = 7; mSmokeCount++; }
	if (mSmokeCount > 3) { mSmokeCount = 3; }

	if (mAnimationList[M rightSkill2]->GetIsPlay() or mAnimationList[M leftSkill2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() > 0.1f - dTime)
		{
			switch (mCurrentAnimation->GetCurrentFrameIndex())
			{
			case 0: 
				SKUL->Invincibilize();
				Attack(2*mMagicalAttackPower, 2, AttackType::Whirlwind);
				mSmokeCount--;
				(new Effect(L"ThiefSmoke", mX, mY, EffectType::Normal))->Scaling(200, 200, 0.7f);
				CAMERA->PanningOn(5);
				Dash(5, true);
				break;

			case 7:
				SKUL->Disinvincibilize();
				break;
			}
		}
	}
}

void PettyThief::SkulSwitch(int indexX, int indexY)
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

void PettyThief::SkulReset()
{
	mCurrentAnimation->Stop();
}


void PettyThief::SetAttackSpeed()
{
	mAnimationList[M rightAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M rightAttack2]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack2]->SetFrameUpdateTime(mAttackSpeed);
}