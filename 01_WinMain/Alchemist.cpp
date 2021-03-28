#include "pch.h"
#include "Alchemist.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "AlchPassive.h"
#include "Animation.h"
#include "Effect.h"

Alchemist::Alchemist(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"Alchemist", Resources(L"/skul/skul_alchemist.bmp"), 800, 1400, 8, 14, true);
	mImage = IMAGEMANAGER->FindImage(L"Alchemist");

	IMAGEMANAGER->LoadFromFile(L"Projectile", Resources(L"/skul/projectile.bmp"), 21, 20, true);
	mProjectile = IMAGEMANAGER->FindImage(L"Projectile");

	IMAGEMANAGER->LoadFromFile(L"Flask", Resources(L"/skul/flask.bmp"), 18, 26, true);
	mFlask = IMAGEMANAGER->FindImage(L"Flask");

	IMAGEMANAGER->LoadFromFile(L"GolemR", Resources(L"/skul/golemRight.bmp"), 1200, 200, 6, 1, true);
	IMAGEMANAGER->LoadFromFile(L"GolemL", Resources(L"/skul/golemLeft.bmp"), 1200, 200, 6,1, true);
	IMAGEMANAGER->LoadFromFile(L"GolemFall", Resources(L"/skul/golemFall.bmp"), 3600, 200, 18, 1, true);

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

	mAnimationList[M rightSkill1] = new Animation(0, 6, 7, 6, false, false, 0.1f);
	mAnimationList[M leftSkill1] = new Animation(0, 7, 7, 7, true, false, 0.1f);
	mAnimationList[M rightSkill2] = new Animation(0, 10, 7, 10, false, false, 0.1f);
	mAnimationList[M leftSkill2] = new Animation(0, 11, 7, 11, true, false, 0.1f);

	mSkill1CoolTime = 0;
	mSkill1Count = 8;
	mSkill2CoolTime = 0;
	mSkill2Count = 5;

	mAttackCoolTime = 0;
	mGolemCoolTime = 0;
}

void Alchemist::Update()
{
	if (LEFT and mPath.size() == 0) SetAnimation(M leftIdle);
	if (RIGHT and mPath.size() == 0) SetAnimation(M rightIdle);

	if (M_LEFT and mPath.size() > 0) { SetAnimation(M leftWalk); }
	if (M_RIGHT and mPath.size() > 0) { SetAnimation(M rightWalk); }

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

	mGolemCoolTime -= dTime; //10�ʿ� �ѹ� �� ��ȯ �нú�
	if (mGolemCoolTime < 0)
	{
		mGolemCoolTime = 10;
		if (RIGHT) {
			Effect* tmp = new Effect(L"GolemR", mX - 100, mY, EffectType::Normal);
			tmp->SetUpdateTime(0.2f);
			tmp->SetNextEffect(L"GolemFall", mX-100, mY);
			new AlchPassive(this, 2*mMagicalAttackPower, mAngle);
		}
		else if (LEFT) {
			Effect* tmp = new Effect(L"GolemL", mX + 100, mY, EffectType::Normal);
			tmp->SetUpdateTime(0.2f);
			tmp->SetNextEffect(L"GolemFall", mX + 100, mY);;
			new AlchPassive(this, 2 * mMagicalAttackPower, mAngle);
		}
	}



	mSpeed = mInitSpeed;
	if (TILE[mIndexY][mIndexX]->GetType() == TileType::Slow)
	{
		mSpeed = mInitSpeed / 2;
	}

	mTileSelect->Update();

	mDashCoolTime -= dTime;
	if (mDashCoolTime < 0) { mDashCoolTime = 0; mDashCount = 0; }

	if (INPUT->GetKeyDown('Z')) //�뽬
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

	mAttackCoolTime -= dTime;
	if (mAttackCoolTime < 0) mAttackCoolTime = 0;
	if (INPUT->GetKey('X'))
	{
		BasicAttack();
	}

	if (INPUT->GetKeyDown('A')) // ���� �ö�ũ
	{
		if (mSkill1Count>0)
		{
			UpdateAngle();
			if (RIGHT) { SetAnimation(M rightSkill1); }
			if (LEFT) { SetAnimation(M leftSkill1); }
		}
		else
		{
			CAMERA->PanningOn(3);
		}
	}
	Skill1();

	if (INPUT->GetKeyDown('S')) // ������ �ö�ũ
	{
		if (mSkill2Count>0)
		{
			UpdateAngle();
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

	//{{ �����ڿ� Ÿ�� üũ ������
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
			new Bullet(mProjectile, "Projectile", this, 1*mMagicalAttackPower, 300, 500, mAngle, BulletType::Straight);
		}
		else
		{
			mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
			new Bullet(mProjectile, "Projectile", this, 1* mMagicalAttackPower, 300, 500, mAngle, BulletType::Straight);
		}
	}
}

void Alchemist::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) {mSkill1Count++; mSkill1CoolTime = 4;}
	if (mSkill1Count > 8) { mSkill1Count = 8;}

	if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
		if (mCurrentAnimation->GetCurrentFrameTime() > 0.1f-dTime and mCurrentAnimation->GetNowFrameX() == 3)
		{
			new Bullet(mFlask, "DiseaseFlask", this, 5* mMagicalAttackPower, 600, 400, mAngle, BulletType::Flask);
			mSkill1Count--;
			CAMERA->PanningOn(5);

		}
}

void Alchemist::Skill2()
{
	mSkill2CoolTime -= dTime;
	if (mSkill2CoolTime < 0) { mSkill2Count++; mSkill2CoolTime = 13; }
	if (mSkill2Count > 5) { mSkill2Count = 5;}

	if (mAnimationList[M rightSkill2]->GetIsPlay() or mAnimationList[M leftSkill2]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() >0.1f- dTime)
		{
			if (mCurrentAnimation->GetNowFrameX() == 3)
			{
				new Bullet(mFlask, "FireFlask", this, 5* mMagicalAttackPower, 600, 400, mAngle, BulletType::Flask);
				mSkill2Count--;
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
		new Bullet(mFlask, "DiseaseFlask", this, mMagicalAttackPower, 300, 30, PI*i/2, BulletType::Flask);
	}
}

void Alchemist::SkulReset()
{
	mCurrentAnimation->Stop();
}
