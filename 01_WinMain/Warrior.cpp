#include "pch.h"
#include "Warrior.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "Animation.h"
#include "Effect.h"
#include "Enemy.h"

Warrior::Warrior(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"Warrior", Resources(L"/skul/skul_warrior.bmp"), 1650, 2400, 11, 16, true);
	mImage = IMAGEMANAGER->FindImage(L"Warrior");

	IMAGEMANAGER->LoadFromFile(L"WarriorHit", Resources(L"/skul/warriorHit.bmp"), 500, 300, 5, 3, true);

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}
void Warrior::Init()
{
	mTileSelect = new TileSelect;

	mAnimationList[M rightIdle] = new Animation(0, 0, 3, 0, false, true, 0.2f);
	mAnimationList[M leftIdle] = new Animation(0, 1, 3, 1, true, true, 0.2f);
	mAnimationList[M rightWalk] = new Animation(0, 2, 7, 2, false, true, 0.2f);
	mAnimationList[M leftWalk] = new Animation(0, 3, 7, 3, true, true, 0.2f);

	mAnimationList[M rightDash] = new Animation(0, 4, 2, 4, false, false, 0.2f);
	mAnimationList[M leftDash] = new Animation(0, 5, 2, 5, true, false, 0.2f);

	mAnimationList[M rightAttack1] = new Animation(0, 6, 8, 6, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M rightAttack2] = new Animation(0, 8, 10, 8, false, false, mAttackSpeed);
	mAnimationList[M leftAttack1] = new Animation(0, 7, 8, 7, true, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M leftAttack2] = new Animation(0, 9, 10, 9, true, false, mAttackSpeed);

	mAnimationList[M rightSkill1] = new Animation(0, 10, 8, 10, false, false, 0.1f);
	mAnimationList[M leftSkill1] = new Animation(0, 11, 8, 11, true, false, 0.1f);
	mAnimationList[M rightSkill2] = new Animation(0, 14, 8, 14, false, false, 0.1f);
	mAnimationList[M leftSkill2] = new Animation(0, 15, 8, 15, true, false, 0.1f);

	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
}

void Warrior::Update()
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
	if (mDashCoolTime < 0) { mDashCoolTime = 0; mDashCount = 0;}

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

	if (INPUT->GetKeyDown('A')) // 올려베기
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

	if (INPUT->GetKeyDown('S')) //  찌르고 넘기기
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

void Warrior::Release()
{
	SafeDelete(mTileSelect);

	for (Animation* elem : mAnimationList)
	{
		SafeDelete(elem);
	}
}

void Warrior::Render(HDC hdc)
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

void Warrior::SetAnimation(int listNum)
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

void Warrior::BasicAttack()
{
	if (mAnimationList[M rightAttack1]->GetIsPlay() or mAnimationList[M leftAttack1]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() > mAttackSpeed - dTime)
		{
			if (mCurrentAnimation->GetCurrentFrameIndex() == 4 )
			{
				Attack(mPhysicalAttackPower, 2, AttackType::Side);
			}
		}
	}	
	else if (mAnimationList[M rightAttack2]->GetIsPlay() or mAnimationList[M leftAttack2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() > mAttackSpeed - dTime)
		{
			if (mCurrentAnimation->GetCurrentFrameIndex() == 4)
			{
				Attack(mPhysicalAttackPower, 2, AttackType::Side);
				CAMERA->PanningOn(3);
			}
		}
	}
}

void Warrior::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) mSkill1CoolTime = 0;

	if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
	{
		mSkill1CoolTime = 5;

		if (mCurrentAnimation->GetCurrentFrameTime() > 0.1f-dTime)
		{
			switch (mCurrentAnimation->GetCurrentFrameIndex())
			{ //3타 다단히트 == 멀리 날릴 수 있다.
			case 3:
				Attack(mPhysicalAttackPower, 3, AttackType::Side);
			case 4:
				Attack(mPhysicalAttackPower, 3, AttackType::Side);
			case 5:
				Attack(mPhysicalAttackPower, 3, AttackType::Side);
				CAMERA->PanningOn(5);

			}
			
			if (mCurrentAnimation->GetCurrentFrameIndex() >= 4)
			{
				int i = mCurrentAnimation->GetCurrentFrameIndex() - 4;
				if (LEFT)
				{
					new Effect(L"WarriorHit", mX - 100 + 50*i, mY - 15*i, EffectType::Normal);
				}
				else if (RIGHT)
				{
					new Effect(L"WarriorHit", mX + 100 - 50 * i, mY - 15 * i, EffectType::Normal);
				}
			}
		}
	}
}

void Warrior::Skill2()
{
	mSkill2CoolTime -= dTime;
	if (mSkill2CoolTime < 0) mSkill2CoolTime = 0;

	if (mAnimationList[M rightSkill2]->GetIsPlay() or mAnimationList[M leftSkill2]->GetIsPlay()) 
	{
		mSkill2CoolTime = 5;

		if (mCurrentAnimation->GetCurrentFrameTime() > 0.1f - dTime)
		{
			switch (mCurrentAnimation->GetCurrentFrameIndex())
			{

			case 1:
				Dash(1); //전진하며 찌르기
				Attack(mPhysicalAttackPower, 3, AttackType::Stab);
				new Effect(L"WarriorHit", mX , mY, EffectType::Normal);
				for (auto elem : Obj->GetObjectList(ObjectLayer::Enemy))
				{
					Enemy* downcast = (Enemy*)elem;
					if (downcast->GetHitTime() == 0.6f)
					{
						mSkill2Targets.push_back(elem);
					}
				}
				break;

			case 4:
				mAngle += PI;
				if (mAngle > PI2) mAngle -= PI2;
				for (GameObject* elem : mSkill2Targets) //찔린 친구들 뒤로 넘기기
				{
					if (TILE[TILELIST->CalcIndexY(mX + cosf(mAngle) * TileSizeX, mY - sinf(mAngle) * TileSizeY)]
						[TILELIST->CalcIndexX(mX + cosf(mAngle) * TileSizeX, mY - sinf(mAngle) * TileSizeY)]->GetType() != TileType::Block)
					{
						elem->SetX(mX + cosf(mAngle) * TileSizeX);
						elem->SetY(mY - sinf(mAngle) * TileSizeY);
						elem->SetIndexX(TILELIST->CalcIndexX(elem->GetX(), elem->GetY()));
						elem->SetIndexY(TILELIST->CalcIndexY(elem->GetX(), elem->GetY()));
						TILE[elem->GetIndexY()][elem->GetIndexX()]->SetObject(elem);
						TILE[elem->GetIndexY()][elem->GetIndexX()]->Update();
					}
					else
					{
						elem->SetX(mX);
						elem->SetY(mY);
						elem->SetIndexX(mIndexX);
						elem->SetIndexY(mIndexY);
						TILE[mIndexY][mIndexX]->SetObject(elem);
						TILE[mIndexY][mIndexX]->Update();
					}
				}
				Attack(mPhysicalAttackPower, 3, AttackType::Side);
				mSkill2Targets.clear();
				CAMERA->PanningOn(5);
				break;

			case 5:
			case 6:
			case 7:
				new Effect(L"WarriorHit", mX - 100 + RAND->RandomInt(200), mY - 100 + RAND->RandomInt(200), EffectType::Normal);
				new Effect(L"WarriorHit", mX - 100 + RAND->RandomInt(200), mY - 100 + RAND->RandomInt(200), EffectType::Normal);
				break;
			}
		
		}
	}
}

void Warrior::SkulSwitch(int indexX, int indexY)
{
	Player::SkulSwitch(indexX, indexY);
	if (LEFT)
	{
		Dash(5);
		Attack(mPhysicalAttackPower, 5, AttackType::Stab);
		SetAnimation(M leftDash);
	}
	if (RIGHT)
	{
		Dash(5);
		Attack(mPhysicalAttackPower, 5, AttackType::Stab);
		SetAnimation(M rightDash);
	}
}

void Warrior::SkulReset()
{
	mCurrentAnimation->Stop();
}

void Warrior::SetAttackSpeed()
{
	mAnimationList[M rightAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M rightAttack2]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack2]->SetFrameUpdateTime(mAttackSpeed);
}
