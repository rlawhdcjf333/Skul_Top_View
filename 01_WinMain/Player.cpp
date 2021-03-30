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
	mSpeed =200;
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
}

void Player::Init()
{
	
}

void Player::Update()
{
	
}

void Player::Release()
{
}

void Player::Render(HDC hdc)
{

}

void Player::Move(float speed)
{
	if (!mPath.empty()) {
		if (mPath.size() <= mPathIndex) //목적지까지 이동 완료
		{
			mPath.clear();
			mPathIndex = 1; 
			if(mIsDash) SKUL->Disinvincibilize();
			mIsDash = false;
		}
		else //이동 중
		{
			int pathX = mPath[mPathIndex]->GetX() + TileSizeX / 2;
			int pathY = mPath[mPathIndex]->GetY() + TileSizeY / 2;
			mMoveAngle = Math::GetAngle(mX, mY, pathX, pathY); 
			mX += speed * cosf(mMoveAngle) * dTime;
			mY -= speed * sinf(mMoveAngle) * dTime;

			if (abs(mX - pathX) < speed * dTime and abs(mY - pathY) < speed * dTime) //오차 보정
			{
				mX = pathX;
				mY = pathY;
				mIndexX = mPath[mPathIndex]->GetIndexX();
				mIndexY = mPath[mPathIndex]->GetIndexY();
				TILE[mIndexY][mIndexX]->SetObject(this);
				TILE[mIndexY][mIndexX]->Update();
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
