#pragma once
#include "pch.h"
#include "Tile.h"
#include "GameObject.h"
#include "Enemy.h"
#include "Player.h"
#include "Effect.h"
#include "Boss.h"

Tile::Tile(Image* pImage, float x, float y, int frameX, int frameY, int sizeX, int sizeY, int indexX, int indexY)
	:mX(x), mY(y), mFrameX(frameX), mFrameY(frameY), mSizeX(sizeX), mSizeY(sizeY), mImage(pImage), mIndexX(indexX), mIndexY(indexY), mObject(nullptr), mIsTileEmpty(false)
{
	mTileType = TileType::Normal;

	mRect = RectMake(mX, mY, mSizeX, mSizeY);
	mDiam = DiamMake(mX, mY, mSizeX, mSizeY);
}

void Tile::Update() //업데이트를 통해 내용 여부 체크
{
	if (!mObjects.empty()) {
		mIsTileEmpty = true;
		for (int a = 0; a < mObjects.size(); a++) {
			if (mObjects[a] == nullptr || mObjects[a]->GetIsDestroy()) {
				mObjects.erase(mObjects.begin() + a);
				--a;
				continue;
			}
			int x = mObjects[a]->GetIndexX();
			int y = mObjects[a]->GetIndexY();
			if (!(x == mIndexX && y == mIndexY)) {
				mObjects.erase(mObjects.begin()+a);
				--a;
				continue;
			}
			Enemy* dumpEnemy =  dynamic_cast<Enemy*>(mObjects[a]);
			if (dumpEnemy) {
				mIsTileEmpty = false;
			}
		}
	}
	else {
		mIsTileEmpty = true;
	}
}


void Tile::Render(HDC hdc)
{
	if (mImage)
	{
		CAMERA->FrameRender(hdc, mImage, mX, mY-mZaxis, mFrameX, mFrameY);

		//CallFont(hdc, 10,
		//	[=]() {
		//		TextOut(hdc, mX+TileSizeX/2 - 5 - CAMERA->GetRect().left, mY +TileSizeY/2-5- CAMERA->GetRect().top, to_wstring(mIndexX).c_str(), to_wstring(mIndexX).size());
		//		TextOut(hdc, mX + TileSizeX / 2 +5 - CAMERA->GetRect().left, mY +TileSizeY/2-5- CAMERA->GetRect().top, to_wstring(mIndexY).c_str(), to_wstring(mIndexY).size());
		//	});

	
		//mImage->ScaleFrameRender(hdc, mX, mY, mFrameX, mFrameY, mSizeX, mSizeY);
		
			
	}
	else
	{
		Gizmo::GetInstance()->DrawDiam(hdc, mDiam, Gizmo::Color::Green);
	}
	if (mAttackTest) {
		mTestTime++;
		if (mTestTime > 10) {
			mTestTime = 0;
			mAttackTest = false;
		}
		SelectRenderBlue(hdc);
	}
}
void Tile::AlphaRender(HDC hdc)
{
	if (mImage)
	{
		CAMERA->AlphaScaleFrameRender(hdc, mImage, mX, mY, mFrameX, mFrameY, mSizeX, mSizeY,0.5f);
		//mImage->ScaleFrameRender(hdc, mX, mY, mFrameX, mFrameY, mSizeX, mSizeY);
	}
	else
	{
		Gizmo::GetInstance()->DrawDiam(hdc, mDiam, Gizmo::Color::Green);
	}
	if (mAttackTest) {
		mTestTime++;
		if (mTestTime > 10) {
			mTestTime = 0;
			mAttackTest = false;
		}
		SelectRenderBlue(hdc);
	}
}

void Tile::SelectRender(HDC hdc)
{
	Gizmo::GetInstance()->DrawRect(hdc, mRect, Gizmo::Color::Red);
}

void Tile::SelectRenderBlue(HDC hdc)
{
	Gizmo::GetInstance()->DrawRect(hdc, mRect, Gizmo::Color::Blue);
}

void Tile::SelectRenderMargenta(HDC hdc)
{
	if (mTileType != TileType::Normal) {
		Gizmo::GetInstance()->DrawDiam(hdc, mDiam, Gizmo::Color::Margenta);
	}
}

void Tile::AttackDamage(int damage) {
	Update();
	for (GameObject* elem : mObjects) {
		Boss* dumpBoss = dynamic_cast<Boss*> (elem);
		if (dumpBoss != nullptr) {
			elem->Damage(damage);
			new Effect(L"SkulHitEffect", elem->GetX(), elem->GetY(), EffectType::Normal);
			continue;
		}
		Enemy* dumpEnemy = dynamic_cast<Enemy*> (elem);
		if (dumpEnemy != nullptr) {
			elem->Damage(damage);
			new Effect(L"SkulHitEffect", elem->GetRect().left, elem->GetRect().top, EffectType::Normal);
		}
	}
	mAttackTest = true;
}
void Tile::EnemyAttack(int damage) {
	Update();
	for (GameObject* elem : mObjects) {
		Player* dumpPlayer = dynamic_cast<Player*> (elem);
		if (dumpPlayer != nullptr) {
			SKUL->Damage(damage);
			SKUL->SetHitTime(0.6f);
		}
	}
	//mAttackTest = true;
}

