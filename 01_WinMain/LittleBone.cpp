#include "pch.h"
#include "LittleBone.h"
#include "TileSelect.h"
#include "LittleHead.h"
#include "Animation.h"
#include "Effect.h"


LittleBone::LittleBone(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"LittleBone", Resources(L"/skul/skul_little_bone.bmp"), 3780, 2400, 27, 24, true);
	mImage = IMAGEMANAGER->FindImage(L"LittleBone");

	IMAGEMANAGER->LoadFromFile(L"Spark", Resources(L"/skul/spark.bmp"),700,100,7,1, true);

	mSizeX = mImage->GetFrameWidth();
	mSizeY = mImage->GetFrameHeight();
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	mRotationCount = 0;
	mIsHead = true;
}

void LittleBone::Init()
{
	mTileSelect = new TileSelect;

	//머가리 있을 때
	mAnimationList[M rightIdle] = new Animation(0, 0, 3, 0, false, true, 0.2f);
	mAnimationList[M leftIdle] = new Animation(0, 11, 3, 11, true, true, 0.2f);
	mAnimationList[M rightWalk] = new Animation(0, 1, 7, 1, false, true, 0.2f);
	mAnimationList[M leftWalk] = new Animation(0, 12, 7, 12, true, true, 0.2f);
	mAnimationList[M rightDash] = new Animation(0, 2, 0, 2, false, false, 0.5f);
	mAnimationList[M leftDash] = new Animation(0, 13, 0, 13, true, false, 0.5f);
	mAnimationList[M rightAttack1] = new Animation(0, 3, 4, 3, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M rightAttack2] = new Animation(0, 4, 3, 4, false, false, mAttackSpeed);
	mAnimationList[M leftAttack1] = new Animation(0, 14, 4, 14, true, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT)SetAnimation(M rightAttack2);
				if (LEFT)SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M leftAttack2] = new Animation(0, 15, 3, 15, true, false, mAttackSpeed);

	mAnimationList[M switchAttack] = new Animation(0, 22, 6, 22, false, false, 0.05f,
		[this]() {Attack(1, 2, AttackType::Whirlwind); mRotationCount++; if (mRotationCount < 6) SetAnimation(M switchAttack);
		else mRotationCount = 0;});
	mAnimationList[M rightSkill1] = new Animation(0, 5, 3, 5, false, false, 0.1f, [this]() {mCurrentAnimation->SetCurrentFrameIndex(1);});
	mAnimationList[M leftSkill1] = new Animation(0, 16, 3, 16, true, false, 0.1f, [this]() {mCurrentAnimation->SetCurrentFrameIndex(1);});
	
	//머가리 없을 때
	mAnimationList[M rightIdleHeadless] = new Animation(0, 6, 3, 6, false, true, 0.2f);
	mAnimationList[M leftIdleHeadless] = new Animation(0, 17, 3, 17, true, true, 0.2f);
	mAnimationList[M rightWalkHeadless] = new Animation(0, 7, 7, 7, false, true, 0.2f);
	mAnimationList[M leftWalkHeadless] = new Animation(0, 18, 7, 18, true, true, 0.2f);
	mAnimationList[M rightDashHeadless] = new Animation(0, 8, 0, 8, false, true, 0.2f);
	mAnimationList[M leftDashHeadless] = new Animation(0, 19, 0, 19, true, true, 0.2f);
	mAnimationList[M rightAttack1Headless] = new Animation(0, 9, 4, 9, false, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if(RIGHT) SetAnimation(M rightAttack2Headless);
				if (LEFT) SetAnimation(M leftAttack2Headless);
			}
		});
	mAnimationList[M rightAttack2Headless] = new Animation(0, 10, 3, 10, false, false, mAttackSpeed);
	mAnimationList[M leftAttack1Headless] = new Animation(0, 20, 4, 20, true, false, mAttackSpeed,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2Headless);
				if (LEFT) SetAnimation(M leftAttack2Headless);
			}
		});
	mAnimationList[M leftAttack2Headless] = new Animation(0, 21, 3, 21, true, false, mAttackSpeed);

	//리스폰
	mAnimationList[M respawning] = new Animation(0, 23, 26, 23, false, false, 0.1f, [this]() {SetAnimation(M rightIdle);});

	SetAnimation(M respawning);// 첫 스타트 리스폰

	mSkill1CoolTime = 0;
	mSkill2CoolTime = 0;
}

void LittleBone::Update()
{
	if (mAnimationList[M respawning]->GetIsPlay())
	{
		mCurrentAnimation->Update();
		return;
	}

	if (mIsHead)
	{
		if (LEFT and mPath.size()==0) SetAnimation(M leftIdle);
		if (RIGHT and mPath.size()==0) SetAnimation(M rightIdle);

		if (M_LEFT and mPath.size() > 0) { SetAnimation(M leftWalk); }
		if (M_RIGHT and mPath.size() > 0) { SetAnimation(M rightWalk); }

		if (LEFT and mIsDash) SetAnimation(M leftDash);
		if (RIGHT and mIsDash) SetAnimation(M rightDash);
	}
	else
	{
		if (LEFT and mPath.size() == 0) SetAnimation(M leftIdleHeadless);
		if (RIGHT and mPath.size() == 0) SetAnimation(M rightIdleHeadless);

		if (M_LEFT and mPath.size() > 0) { SetAnimation(M leftWalkHeadless); }
		if (M_RIGHT and mPath.size() > 0) { SetAnimation(M rightWalkHeadless); }

		if (LEFT and mIsDash) SetAnimation(M leftDashHeadless);
		if (RIGHT and mIsDash) SetAnimation(M rightDashHeadless);
	}

	mSpeed = mInitSpeed;
	if (TILE[mIndexY][mIndexX]->GetType() == TileType::Slow)
	{
		mSpeed = mInitSpeed / 2;
	}

	mTileSelect->Update();



	if (INPUT->GetKey('X')) //기본공격
	{
		if (!mAnimationList[M rightAttack1]->GetIsPlay() and !mAnimationList[M rightAttack2]->GetIsPlay()
			and !mAnimationList[M leftAttack1]->GetIsPlay() and !mAnimationList[M leftAttack2]->GetIsPlay()
			and !mAnimationList[M rightAttack1Headless]->GetIsPlay() and !mAnimationList[M rightAttack2Headless]->GetIsPlay()
			and !mAnimationList[M leftAttack1Headless]->GetIsPlay() and !mAnimationList[M leftAttack2Headless]->GetIsPlay())
		{
			UpdateAngle();
		}

		if (mIsHead)
		{
			if (RIGHT) { SetAnimation(M rightAttack1);}
			if (LEFT) { SetAnimation(M leftAttack1); }
		}
		else
		{
			if (RIGHT) { SetAnimation(M rightAttack1Headless); }
			if (LEFT) { SetAnimation(M leftAttack1Headless); }
		}
	}
	BasicAttack();

	if (INPUT->GetKeyDown('A') and mIsHead and mSkill1CoolTime==0) //머가리 던지기
	{
		mCurrentAnimation->Stop();
		mIsHead = false;
		UpdateAngle();
		new LittleHead(this, 2*mPhysicalAttackPower, mAngle, 500);

		if (RIGHT) SetAnimation(M rightSkill1);
		if (LEFT) SetAnimation(M leftSkill1);
	}
	Skill1();

	if (INPUT->GetKeyDown('S') and !mIsHead) //머가리 줍기
	{
		GameObject* head = Obj->FindObject(ObjectLayer::Player_Bullet, "LittleHead");
		if (head == nullptr) {mIsHead = true; return;}

		mX = head->GetX();
		mY = head->GetY();
		mIndexX = TileList::GetInstance()->CalcIndexX(mX, mY);
		mIndexY = TileList::GetInstance()->CalcIndexY(mX, mY);
		mPath.clear(); mPathIndex = 1;
		head->SetIsDestroy(true);
		new Effect(L"Spark", mX, mY, EffectType::Normal);
		mIsHead = true;
	}

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

void LittleBone::Release()
{
	SafeDelete(mTileSelect);

	for (Animation* elem : mAnimationList)
	{
		SafeDelete(elem);
	}
}

void LittleBone::Render(HDC hdc)
{
	CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top+25, mCurrentAnimation->GetNowFrameX(), mCurrentAnimation->GetNowFrameY(), mSizeX, mSizeY);

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

void LittleBone::SetAnimation(int listNum)
{
	if (mAnimationList[M rightAttack1]->GetIsPlay()) return;
	if (mAnimationList[M rightAttack2]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack1]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack2]->GetIsPlay()) return;

	if (mAnimationList[M rightAttack1Headless]->GetIsPlay()) return;
	if (mAnimationList[M rightAttack2Headless]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack1Headless]->GetIsPlay()) return;
	if (mAnimationList[M leftAttack2Headless]->GetIsPlay()) return;

	if (mAnimationList[M rightDash]->GetIsPlay()) return;
	if (mAnimationList[M leftDash]->GetIsPlay()) return;
	if (mAnimationList[M switchAttack]->GetIsPlay())
	{
		mPath.clear();
		return;
	}
	if (mAnimationList[M rightSkill1]->GetIsPlay()) return;
	if (mAnimationList[M leftSkill1]->GetIsPlay()) return;


	mCurrentAnimation = mAnimationList[listNum];
	mCurrentAnimation->Play();
}

void LittleBone::Skill1()
{
	mSkill1CoolTime -= dTime;
	if (mSkill1CoolTime < 0) mSkill1CoolTime = 0;

	if (mAnimationList[M rightSkill1]->GetIsPlay() or mAnimationList[M leftSkill1]->GetIsPlay())
	{
		mSkill1CoolTime = 4;
	}
}

void LittleBone::BasicAttack()
{
	if (mAnimationList[M rightAttack1]->GetIsPlay() or mAnimationList[M leftAttack1]->GetIsPlay()
		or mAnimationList[M rightAttack2]->GetIsPlay() or mAnimationList[M leftAttack2]->GetIsPlay()
		or mAnimationList[M rightAttack1Headless]->GetIsPlay() or mAnimationList[M leftAttack1Headless]->GetIsPlay()
		or mAnimationList[M rightAttack2Headless]->GetIsPlay() or mAnimationList[M leftAttack2Headless]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() > mAttackSpeed-dTime and mCurrentAnimation->GetNowFrameX() == 1)
		{
			Attack(mPhysicalAttackPower, 1, AttackType::Side);
		}
	}

}
void LittleBone::SkulSwitch(int indexX, int indexY)
{
	
	SetAnimation(M switchAttack);// 일단 변경되서 들어오면 빙글빙글
	Player::SkulSwitch(indexX, indexY);
}
void LittleBone::SkulReset() {
	mCurrentAnimation->Stop();
	GameObject* head = Obj->FindObject(ObjectLayer::Player_Bullet, "LittleHead");
	if (head) head->SetIsDestroy(true);
	mIsHead = true;
}

void LittleBone::SetAttackSpeed()
{
	mAnimationList[M rightAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M rightAttack2]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack1]->SetFrameUpdateTime(mAttackSpeed);
	mAnimationList[M leftAttack2]->SetFrameUpdateTime(mAttackSpeed);
}

