#include "pch.h"
#include "Player.h"
#include "TileSelect.h"
#include "Bullet.h"
#include "Effect.h"

Player::Player(int indexX, int indexY, float sizeX, float sizeY)
	:GameObject("Player")
{
	mIndexX = indexX;
	mIndexY = indexY;
	mX = TILE[mIndexY][mIndexX]->GetX()+TileSizeX/2;
	mY = TILE[mIndexY][mIndexX]->GetY()+TileSizeY/2;
	mSpeed = mInitSpeed=200;
	mAngle = 0;
	mSizeX = sizeX;
	mSizeY = sizeY;
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
	mHitBox = RectMakeBottom(mX, mY, 30, 30);

	mIsDash = false;
	mDashCoolTime = 0;
	mInitDashCoolTime=2.f;
	mDashCount = 0;

	mName = "player";

	mPhysicalAttackPower = 1;
	mMagicalAttackPower = 1;
	mAttackSpeed = 0.1f;
}

void Player::Init()
{
	IMAGEMANAGER->LoadFromFile(L"Sans", Resources(L"Sans.bmp"), 92, 30, 4, 1, true);
	mImage = IMAGEMANAGER->GetInstance()->FindImage(L"Sans");
	mTileSelect = new TileSelect;
}

void Player::Update()
{
	
	mSpeed = mInitSpeed;
	if (TILE[mIndexY][mIndexX]->GetType() == TileType::Slow)
	{
		mSpeed = mInitSpeed / 2;
	}

	mTileSelect->Update();

	if (mIsDash)
	{
		Move(5*mInitSpeed); //대쉬 중 5배 가속
	}
	else
	{
		if (Input::GetInstance()->GetKey(VK_RBUTTON) and mTileSelect) //대쉬 중이 아닐 경우에만 클릭 이동 활성화 == 대쉬 중에 이동캔슬 안됨
		{
			if (PathFinder::GetInstance()->FindPath(TILE, mPath, mIndexX, mIndexY,
				mTileSelect->GetIndexX(), mTileSelect->GetIndexY())) mPathIndex = 1;
		}
		Move(mSpeed);
	}

	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);

}

void Player::Release()
{
	SafeDelete(mTileSelect);
}

void Player::Render(HDC hdc)
{
	CAMERA->ScaleFrameRender(hdc, mImage, mRect.left, mRect.top, 0, 0, mSizeX, mSizeY);
	mTileSelect->Render(hdc);

	//{{ 개발자용 타일 체크 렌더링
	TILE[mIndexY][mIndexX]->SelectRender(hdc);

	for (Tile* elem : mPath)
	{
		elem->SelectRender(hdc);
	}
	//}}
}

void Player::Move(float speed)
{
	if (!mPath.empty()) {
		if (mPath.size() <= mPathIndex) //목적지까지 이동 완료
		{
			mPath.clear();
			mPathIndex = 1; 
			mIsDash = false; SKUL->Disinvincibilize();
		}
		else //이동 중
		{
			int pathX = mPath[mPathIndex]->GetX() + TileSizeX / 2;
			int pathY = mPath[mPathIndex]->GetY() + TileSizeY / 2;
			mMoveAngle = Math::GetAngle(mX, mY, pathX, pathY); //앵글 거리 계산이 0이 나올때 리턴 0으로 막음
			mX += speed * cosf(mMoveAngle) * dTime;
			mY -= speed * sinf(mMoveAngle) * dTime;

			if (abs(mX - pathX) < speed * dTime and abs(mY - pathY) < speed * dTime) //오차 보정
			{
				mX = pathX;
				mY = pathY;
				mIndexX = mPath[mPathIndex]->GetIndexX();
				mIndexY = mPath[mPathIndex]->GetIndexY();
				TILE[mIndexY][mIndexX]->SetObject(this);
				mPathIndex++;
			}

		}
	}
}

void Player::Dash(int dist, bool isBack)
{
	//커서 방향으로 int dist의 타일 블록 크기만큼 대쉬
	mPath.clear();
	mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());

	if (isBack)
	{
		if(mAngle< PI) mAngle += PI;
		else if (mAngle >= PI) mAngle -= PI;
	}

	if (mAngle >= PI2-(PI/8) or mAngle <(PI/8)) //우향
	{
		for (int i = 0; i < dist+1; i++) {
			if (mIndexX + i < TILESizeX and mIndexY - i >= 0)
			{
				mPath.push_back(TILE[mIndexY - i][mIndexX + i]);
			}
		}
	}
	else if (mAngle >= (PI / 8) and mAngle < (3 * PI / 8)) //우상향
	{
		for (int i = 0; i < dist + 1; i++) { 
			if (mIndexY - i >= 0 and mIndexY - i < TILESizeY)
			{
				mPath.push_back(TILE[mIndexY - i][mIndexX]);
			}
		}
	}
	else if (mAngle >= (3 * PI / 8) and mAngle < (5 * PI / 8)) //상향
	{
		for (int i = 0; i < dist + 1; i++) { 
			if (mIndexX - i >= 0 and mIndexY - i >= 0)
			{
				mPath.push_back(TILE[mIndexY - i][mIndexX - i]);
			}
		}
	}
	else if (mAngle >= (5 * PI / 8) and mAngle < (7 * PI / 8)) //좌상향
	{
		for (int i = 0; i < dist + 1; i++) {
			if (mIndexX - i >= 0 )
			{
				mPath.push_back(TILE[mIndexY][mIndexX - i]);
			}
		}
	}
	else if (mAngle >= (7 * PI / 8) and mAngle < (9 * PI / 8)) //좌향
	{
		for (int i = 0; i < dist+1; i++) {
			if (mIndexX - i >= 0 and mIndexY + i < TILESizeY)
			{
				mPath.push_back(TILE[mIndexY + i][mIndexX - i]);
			}
		}
	}
	else if (mAngle >= (9 * PI / 8) and mAngle < (11 * PI / 8)) //좌하향
	{
		for (int i = 0; i < dist + 1; i++) { 
			if (mIndexY + i < TILESizeY)
			{
				mPath.push_back(TILE[mIndexY + i][mIndexX]);
			}
		}
	}
	else if (mAngle >= (11 * PI / 8) and mAngle < (13 * PI / 8)) //하향
	{
		for (int i = 0; i < dist + 1; i++) { 
			if (mIndexX + i < TILESizeX and mIndexY +i < TILESizeY)
			{
				mPath.push_back(TILE[mIndexY + i][mIndexX + i]);
			}
		}
	}
	else if (mAngle >= (13 * PI / 8) and mAngle < (15 * PI / 8)) //우하향
	{
		for (int i = 0; i < dist + 1; i++) { 
			if (mIndexX + i < TILESizeX)
			{
				mPath.push_back(TILE[mIndexY][mIndexX + i]);
			}
		}
	}
	
	for (Tile* elem : mPath) //대쉬 경로에 막힌 길이 존재하면 막힌 타일 이전까지만 대쉬
	{
		if (elem->GetType() == TileType::Block)
		{
			mPath.assign(mPath.begin(), find(mPath.begin(), mPath.end(), elem));
		}
	}

	if(RIGHT) (new Effect(L"DashRightEffect", mX, mY, EffectType::Normal))->SetUpdateTime(0.05f); 
	if(LEFT) (new Effect(L"DashLeftEffect", mX, mY, EffectType::Reverse))->SetUpdateTime(0.05f);
	mPathIndex = 1; mIsDash = true; SKUL->Invincibilize();


}


void Player::Attack(int damage, int range, AttackType type, bool isBack)
{
	if (isBack)
	{
		if (mAngle < PI) mAngle += PI;
		else if (mAngle >= PI) mAngle -= PI;
	}

	switch (type) {
		case AttackType::Side:
			for (int y = mIndexY - range; y <= mIndexY + range; y++) {
				for (int x = mIndexX - range; x <= mIndexX + range; x++) {
					if (y <= 0 || y > TILESizeY || x <= 0 || x > TILESizeX) {
						continue;
					}

					if (mAngle > 7 * PI / 4 or mAngle <= PI / 4) //우측
					{
						if (y - x > mIndexY - mIndexX) continue;
						TILE[y][x]->AttackDamage(damage);
					}
					else if (mAngle > 3 * PI / 4 and mAngle <= 5 * PI / 4) //좌측
					{
						if (y - x < mIndexY - mIndexX) continue;
						TILE[y][x]->AttackDamage(damage);
					}
					else if (mAngle > PI / 4 and mAngle <= 3 * PI / 4) //상방
					{
						if (y + x > mIndexY + mIndexX) continue;
						TILE[y][x]->AttackDamage(damage);
					}
					else if (mAngle > 5 * PI / 4 and mAngle <= 7 * PI / 4)//하방
					{
						if (y + x < mIndexY + mIndexX) continue;
						TILE[y][x]->AttackDamage(damage);
					}
				}
			}
		break;

		case AttackType::Stab:
		if (mAngle >= PI2 - (PI / 8) or mAngle < (PI / 8)) //우향
		{
			for (int i = 0; i < range + 1; i++) {
				if (mIndexX + i < TILESizeX and mIndexY - i >= 0)
				{
					if (TILE[mIndexY - i][mIndexX + i]->GetType() == TileType::Block)
						break;
					TILE[mIndexY - i][mIndexX + i]->AttackDamage(damage);
					TILE[mIndexY- i][mIndexX-1 + i]->AttackDamage(damage);
					TILE[mIndexY +1- i][mIndexX + i]->AttackDamage(damage);
				}
			}
		}
		else if (mAngle >= (PI / 8) and mAngle < (3 * PI / 8)) //우상향
		{
			for (int i = 0; i < range + 1; i++) {
				if (mIndexY - i >= 0 and mIndexY - i < TILESizeY)
				{
					if (TILE[mIndexY - i][mIndexX]->GetType() == TileType::Block)
						break;
					TILE[mIndexY - i][mIndexX]->AttackDamage(damage);
					TILE[mIndexY - i][mIndexX-1]->AttackDamage(damage);
					TILE[mIndexY - i][mIndexX+1]->AttackDamage(damage);
				}
			}
		}
		else if (mAngle >= (3 * PI / 8) and mAngle < (5 * PI / 8)) //상향
		{
			for (int i = 0; i < range + 1; i++) {
				if (mIndexX - i >= 0 and mIndexY - i >= 0)
				{
					if (TILE[mIndexY - i][mIndexX - i]->GetType() == TileType::Block)
						break;
					TILE[mIndexY - i][mIndexX - i]->AttackDamage(damage);
					TILE[mIndexY - i+1][mIndexX - i]->AttackDamage(damage);
					TILE[mIndexY - i][mIndexX+1 - i]->AttackDamage(damage);
				}
			}
		}
		else if (mAngle >= (5 * PI / 8) and mAngle < (7 * PI / 8)) //좌상향
		{
			for (int i = 0; i < range + 1; i++) {
				if (mIndexX - i >= 0)
				{
					if (TILE[mIndexY][mIndexX - i]->GetType() == TileType::Block)
						break;
					TILE[mIndexY][mIndexX - i]->AttackDamage(damage);
					TILE[mIndexY-1][mIndexX - i]->AttackDamage(damage);
					TILE[mIndexY+1][mIndexX - i]->AttackDamage(damage);
				}
			}
		}
		else if (mAngle >= (7 * PI / 8) and mAngle < (9 * PI / 8)) //좌향
		{
			for (int i = 0; i < range + 1; i++) {
				if (mIndexX - i >= 0 and mIndexY + i < TILESizeY)
				{
					if (TILE[mIndexY + i][mIndexX - i]->GetType() == TileType::Block)
						break;
					TILE[mIndexY + i][mIndexX - i]->AttackDamage(damage);
					TILE[mIndexY + i][mIndexX+1 - i]->AttackDamage(damage);
					TILE[mIndexY-1 + i][mIndexX - i]->AttackDamage(damage);
				}
			}
		}
		else if (mAngle >= (9 * PI / 8) and mAngle < (11 * PI / 8)) //좌하향
		{
			for (int i = 0; i < range + 1; i++) {
				if (mIndexY + i < TILESizeY)
				{
					if (TILE[mIndexY + i][mIndexX]->GetType() == TileType::Block)
						break;
					TILE[mIndexY + i][mIndexX]->AttackDamage(damage);
					TILE[mIndexY + i][mIndexX-1]->AttackDamage(damage);
					TILE[mIndexY + i][mIndexX+1]->AttackDamage(damage);
				}
			}
		}
		else if (mAngle >= (11 * PI / 8) and mAngle < (13 * PI / 8)) //하향
		{
			for (int i = 0; i < range + 1; i++) {
				if (mIndexX + i < TILESizeX and mIndexY + i < TILESizeY)
				{
					if (TILE[mIndexY + i][mIndexX + i]->GetType() == TileType::Block)
						break;
					TILE[mIndexY + i][mIndexX + i]->AttackDamage(damage);
					TILE[mIndexY + i][mIndexX-1 + i]->AttackDamage(damage);
					TILE[mIndexY-1 + i][mIndexX + i]->AttackDamage(damage);
				}
			}
		}
		else if (mAngle >= (13 * PI / 8) and mAngle < (15 * PI / 8)) //우하향
		{
			for (int i = 0; i < range + 1; i++) {
				if (mIndexX + i < TILESizeX)
				{
					if (TILE[mIndexY][mIndexX + i]->GetType() == TileType::Block)
						break;
					TILE[mIndexY][mIndexX + i]->AttackDamage(damage);
					TILE[mIndexY+1][mIndexX + i]->AttackDamage(damage);
					TILE[mIndexY-1][mIndexX + i]->AttackDamage(damage);
				}
			}
		}
		break;

		case AttackType::Whirlwind:
		for (int y = mIndexY - range; y <= mIndexY + range; y++) {
			for (int x = mIndexX - range; x <= mIndexX + range; x++) {
				if (y <=0 || y>TILESizeY || x<=0 || x>TILESizeX) {
						continue;
				}
				TILE[y][x]->AttackDamage(damage);
			}
		}
		break;

		case AttackType::RangedAttack:
			new Bullet(nullptr,"Bullet",this,damage,300,range,mAngle,BulletType::Straight);
		break;
	
	}
}

void Player::PhysicalAttackBuff(int percentage, float buffDuration) //물리공격력 버프
{
	int plus = mPhysicalAttackPower * percentage / 100;
	mPhysicalAttackPower += plus;
	if (SKUL->GetAlterSkul()) SKUL->GetAlterSkul()->SetPhysicalAttackPower(SKUL->GetAlterSkul()->GetPhysicalAttackPower()+plus);

	SKUL->RegBuff([this, plus]() {
		mPhysicalAttackPower -= plus; if (SKUL->GetAlterSkul()) SKUL->GetAlterSkul()->SetPhysicalAttackPower(SKUL->GetAlterSkul()->GetPhysicalAttackPower() - plus);
		}, buffDuration);

}

void Player::AttackSpeedBuff(int percentage, float buffDuration) //공속버프
{
	float upSpeed = (mAttackSpeed * percentage) / (100+percentage);
	mAttackSpeed -= upSpeed;
	if (SKUL->GetAlterSkul()) SKUL->GetAlterSkul()->AttackSpeedSet(mAttackSpeed); 

	SKUL->GetCurrentSkul()->SetAttackSpeed();
	if (SKUL->GetAlterSkul()) SKUL->GetAlterSkul()->SetAttackSpeed();

	SKUL->RegBuff([this, upSpeed]() {
		mAttackSpeed += upSpeed; SKUL->GetCurrentSkul()->AttackSpeedSet(mAttackSpeed); if (SKUL->GetAlterSkul()) SKUL->GetAlterSkul()->AttackSpeedSet(mAttackSpeed);
		SKUL->GetCurrentSkul()->SetAttackSpeed(); if (SKUL->GetAlterSkul()) SKUL->GetAlterSkul()->SetAttackSpeed();}, buffDuration);

}


void Player::SkulSwitch(int indexX, int indexY)
{
	mIndexX = indexX;
	mIndexY = indexY;
	mX = TILE[mIndexY][mIndexX]->GetX() + TileSizeX / 2;
	mY = TILE[mIndexY][mIndexX]->GetY() + TileSizeY / 2;
	mAngle = Math::GetAngle(mX, mY, CAMERA->CameraMouseX(), CAMERA->CameraMouseY());
	mPath.clear();
	mPathIndex = 1;
}
