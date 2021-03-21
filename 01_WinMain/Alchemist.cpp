#include "pch.h"
#include "Alchemist.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "Animation.h"

Alchemist::Alchemist(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"Alchemist", Resources(L"/skul/skul_alchemist.bmp"), 800, 1400, 8, 14, true);
	mImage = IMAGEMANAGER->FindImage(L"Alchemist");

	IMAGEMANAGER->LoadFromFile(L"Projectile", Resources(L"/skul/projectile.bmp"), 30,30, true);
	mProjectile = IMAGEMANAGER->FindImage(L"Projectile");

	IMAGEMANAGER->LoadFromFile(L"DiseaseFlask", Resources(L"/skul/diseaseFlask.bmp"), 15, 23, true);
	mFlask1 = IMAGEMANAGER->FindImage(L"DiseaseFlask");

	IMAGEMANAGER->LoadFromFile(L"FireFlask", Resources(L"/skul/fireFlask.bmp"), 15, 23, true);
	mFlask2 = IMAGEMANAGER->FindImage(L"FireFlask");

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}
void Alchemist::Init()
{
	mTileSelect = new TileSelect;

	mAnimationList[M rightIdle] = new Animation(0, 0, 5, 0, false, true, 0.1f);
	mAnimationList[M leftIdle] = new Animation(0, 1, 5, 1, true, true, 0.1f);
	mAnimationList[M rightWalk] = new Animation(0, 2, 5, 2, false, true, 0.1f);
	mAnimationList[M leftWalk] = new Animation(0, 3, 5, 3, true, true, 0.1f);

	mAnimationList[M rightDash] = new Animation(0, 4, 6, 4, false, false, 0.05f);
	mAnimationList[M leftDash] = new Animation(0, 5, 6, 5, true, false, 0.05f);

	mAnimationList[M rightSkill1] = new Animation(0, 6, 7, 6, false, false, 0.03f);
	mAnimationList[M leftSkill1] = new Animation(0, 7, 7, 7, true, false, 0.03f);
	mAnimationList[M rightSkill2] = new Animation(0, 10, 7, 10, false, false, 0.03f);
	mAnimationList[M leftSkill2] = new Animation(0, 11, 7, 11, true, false, 0.03f);

	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
	mAttackCoolTime = 0;

}

void Alchemist::Update()
{
	if (LEFT and mPath.size() == 0) SetAnimation(M leftIdle);
	if (RIGHT and mPath.size() == 0) SetAnimation(M rightIdle);

	if (LEFT and mPath.size() > 0) SetAnimation(M leftWalk);
	if (RIGHT and mPath.size() > 0) SetAnimation(M rightWalk);

	auto EnemyList = Obj->GetObjectList(ObjectLayer::Enemy);
	if (EnemyList.size() > 0)
	{
		mMinDistance = Math::GetDistance(mX, mY, EnemyList[0]->GetX(), EnemyList[0]->GetY());
		mAttackTarget = EnemyList[0];

		for (GameObject* elem : EnemyList)
		{
			float dist = Math::GetDistance(mX, mY, elem->GetX(), elem->GetY());
			if (mMinDistance > dist)
			{
				mMinDistance = dist;
				mAttackTarget = elem;
			}
		}
	}
	else
	{
		mAttackTarget = nullptr;
	}


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

	mAttackCoolTime -= dTime;
	if (mAttackCoolTime < 0) mAttackCoolTime = 0;
	if (INPUT->GetKey('X') or INPUT->GetKeyDown('X'))
	{
		BasicAttack();
	}

	if (INPUT->GetKeyDown('A')) // 역병 플라스크
	{
		if (mSkill1CoolTime == 0)
		{
			mSkill1CoolTime = 4;
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

	if (INPUT->GetKeyDown('S')) // 불지옥 플라스크
	{
		if (mSkill2CoolTime == 0)
		{
			mSkill2CoolTime = 13;
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

void Alchemist::Release()
{
	SafeDelete(mTileSelect);

	for (Animation* elem : mAnimationList)
	{
		SafeDelete(elem);
	}
}

void Alchemist::Render(HDC hdc)
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

void Alchemist::SetAnimation(int listNum)
{
	if (mAnimationList[M rightDash]->GetIsPlay()) return;
	if (mAnimationList[M leftDash]->GetIsPlay()) return;

	if (mAnimationList[M leftSkill1]->GetIsPlay()) return;
	if (mAnimationList[M leftSkill2]->GetIsPlay()) return;
	if (mAnimationList[M rightSkill1]->GetIsPlay()) return;
	if (mAnimationList[M rightSkill2]->GetIsPlay()) return;

	mCurrentAnimation = mAnimationList[listNum];
	mCurrentAnimation->Play();
}

void Alchemist::BasicAttack()
{
	if (mAttackCoolTime ==0)
	{
		mAttackCoolTime = 0.3f;
		if (mAttackTarget)
		{
			mAngle = Math::GetAngle(mX, mY, mAttackTarget->GetX(), mAttackTarget->GetY());
			new Bullet(mProjectile, "Projectile", this, 1, 300, 500, mAngle, BulletType::Straight);
		}
		else
		{
			mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
			new Bullet(mProjectile, "Projectile", this, 1, 300, 500, mAngle, BulletType::Straight);
		}
	}
}

void Alchemist::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) mSkill1CoolTime = 0;

	if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime and mCurrentAnimation->GetNowFrameX() == 3)
		{
			mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
			new Bullet(mFlask1, "DiseaseFlask", this, 5, 600, 400, mAngle, BulletType::Flask);
			CAMERA->PanningOn(5);

		}
}

void Alchemist::Skill2()
{
	mSkill2CoolTime -= dTime;
	if (mSkill2CoolTime < 0) mSkill2CoolTime = 0;

	if (mAnimationList[M rightSkill2]->GetIsPlay() or mAnimationList[M leftSkill2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime)
		{
			if (mCurrentAnimation->GetNowFrameX() == 3)
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				new Bullet(mFlask2, "FireFlask", this, 5, 600, 400, mAngle, BulletType::Flask);
				CAMERA->PanningOn(5);
			}
		}
	}
}

void Alchemist::SkulSwitch(int indexX, int indexY)
{
	Player::SkulSwitch(indexX, indexY);
	if (LEFT)
	{
		SetAnimation(M leftIdle);
	}
	if (RIGHT)
	{
		SetAnimation(M rightIdle);
	}
	for (int i = 0; i < 4; i++)
	{
		new Bullet(Random::GetInstance()->RandomInt(50) > 25 ? mFlask1 : mFlask2, "Flask", this, 1, 300, 30, PI*i/2, BulletType::Flask);
	}
}

void Alchemist::SkulReset()
{
	mCurrentAnimation->Stop();
}
