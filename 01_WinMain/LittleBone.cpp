#include "pch.h"
#include "LittleBone.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "Animation.h"

LittleBone::LittleBone(int indexX, int indexY, float sizeX, float sizeY)
	:Player(indexX, indexY, sizeX, sizeY)
{
	IMAGEMANAGER->LoadFromFile(L"LittleBone", Resources(L"/skul/skul_little_bone.bmp"), 3780, 2400, 27, 24, true);
	mImage = IMAGEMANAGER->FindImage(L"LittleBone");

	IMAGEMANAGER->LoadFromFile(L"HeadImage", Resources(L"/skul/head.bmp"), 15, 13, true);
	mHeadImage = IMAGEMANAGER->FindImage(L"HeadImage");

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
	mAnimationList[M rightAttack1] = new Animation(0, 3, 4, 3, false, false, 0.1f, 
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2);
				if (LEFT) SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M rightAttack2] = new Animation(0, 4, 3, 4, false, false, 0.1f);
	mAnimationList[M leftAttack1] = new Animation(0, 14, 4, 14, true, false, 0.1f, 
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT)SetAnimation(M rightAttack2);
				if (LEFT)SetAnimation(M leftAttack2);
			}
		});
	mAnimationList[M leftAttack2] = new Animation(0, 15, 3, 15, true, false, 0.1f);

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
	mAnimationList[M rightAttack1Headless] = new Animation(0, 9, 4, 9, false, false, 0.1f,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if(RIGHT) SetAnimation(M rightAttack2Headless);
				if (LEFT) SetAnimation(M leftAttack2Headless);
			}
		});
	mAnimationList[M rightAttack2Headless] = new Animation(0, 10, 3, 10, false, false, 0.1f);
	mAnimationList[M leftAttack1Headless] = new Animation(0, 20, 4, 20, true, false, 0.1f,
		[this]() {
			if (INPUT->GetKey('X'))
			{
				mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
				if (RIGHT) SetAnimation(M rightAttack2Headless);
				if (LEFT) SetAnimation(M leftAttack2Headless);
			}
		});
	mAnimationList[M leftAttack2Headless] = new Animation(0, 21, 3, 21, true, false, 0.1f);

	//리스폰
	mAnimationList[M respawning] = new Animation(0, 23, 26, 23, false, false, 0.1f, [this]() {SetAnimation(M rightIdle);});

	SetAnimation(M respawning);
}

void LittleBone::Update()
{
	if (mIsHead)
	{
		if (LEFT and mPath.size()==0) SetAnimation(M leftIdle);
		if (RIGHT and mPath.size()==0) SetAnimation(M rightIdle);

		if (LEFT and mPath.size() > 0) SetAnimation(M leftWalk);
		if (RIGHT and mPath.size() > 0) SetAnimation(M rightWalk);

		if (LEFT and mIsDash) SetAnimation(M leftDash);
		if (RIGHT and mIsDash) SetAnimation(M rightDash);
	}
	else
	{
		if (LEFT and mPath.size() == 0) SetAnimation(M leftIdleHeadless);
		if (RIGHT and mPath.size() == 0) SetAnimation(M rightIdleHeadless);

		if (LEFT and mPath.size() > 0) SetAnimation(M leftWalkHeadless);
		if (RIGHT and mPath.size() > 0) SetAnimation(M rightWalkHeadless);

		if (LEFT and mIsDash) SetAnimation(M leftDashHeadless);
		if (RIGHT and mIsDash) SetAnimation(M rightDashHeadless);
	}

	mSpeed = mInitSpeed;
	if (TILE[mIndexY][mIndexX]->GetType() == TileType::Slow)
	{
		mSpeed = mInitSpeed / 2;
	}

	mTileSelect->Update();

	if (INPUT->GetKeyDown(VK_SPACE) and mIsHead)
	{
		SetAnimation(M switchAttack);
		CAMERA->PanningOn(5);
	}

	if (INPUT->GetKey('X')) //기본공격
	{
		mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
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

	if (INPUT->GetKeyDown('A')) //머가리 던지기
	{
		Skill1();
		mIsHead = !mIsHead;
	}
	if (INPUT->GetKeyDown('Z')) //대쉬
	{
		Dash(5);
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
	if (mAnimationList[M switchAttack]->GetIsPlay()) return;
	if (mAnimationList[M rightSkill1]->GetIsPlay()) return;
	if (mAnimationList[M leftSkill1]->GetIsPlay()) return;

	if (mAnimationList[M respawning]->GetIsPlay()) return;

	mCurrentAnimation = mAnimationList[listNum];
	mCurrentAnimation->Play();
}

void LittleBone::Skill1()
{
	if (mIsHead)
	{
		mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());

		new Bullet(mHeadImage, "skulHead", this, 1, 500, 500, mAngle, BulletType::SkulHead);

		if (RIGHT) SetAnimation(M rightSkill1);
		if (LEFT) SetAnimation(M leftSkill1);
	}
	else
	{
		GameObject* head = Obj->FindObject(ObjectLayer::Player_Bullet, "skulHead");

		if (head->GetX() >= CAMERA->GetRect().right
			or head->GetX() <= CAMERA->GetRect().left
			or head->GetY() >= CAMERA->GetRect().bottom
			or head->GetY() <= CAMERA->GetRect().top)
		{
			head->SetIsDestroy(true);
			return;
		}

		mX = head->GetX();
		mY = head->GetY();
		mIndexX = TileList::GetInstance()->CalcIndexX(mX, mY);
		mIndexY = TileList::GetInstance()->CalcIndexY(mX, mY);
		mPath.clear(); mPathIndex = 1;
		head->SetIsDestroy(true);
	}
}

void LittleBone::BasicAttack()
{
	if (mAnimationList[M rightAttack1]->GetIsPlay() or mAnimationList[M leftAttack1]->GetIsPlay()
		or mAnimationList[M rightAttack2]->GetIsPlay() or mAnimationList[M leftAttack2]->GetIsPlay()
		or mAnimationList[M rightAttack1Headless]->GetIsPlay() or mAnimationList[M leftAttack1Headless]->GetIsPlay()
		or mAnimationList[M rightAttack2Headless]->GetIsPlay() or mAnimationList[M leftAttack2Headless]->GetIsPlay())
	{
		if (mCurrentAnimation->GetCurrentFrameTime() < dTime and mCurrentAnimation->GetNowFrameX() == 1)
		{
			Attack(1, 1, AttackType::Side);
		}
	}

}
