#include "pch.h"
#include "Dumb.h"

Dumb::Dumb():GameObject("Dumb"),mIsHit(false) {
	mHp = 10;
	mIndexX = 38;
	mIndexY = 38;
	mX = TILE[mIndexY][mIndexX]->GetX() + 30;
	mY = TILE[mIndexY][mIndexX]->GetY() + 15;
	mSizeX = 30;
	mSizeY = 30;
	mRect = RectMakeBottom(mX, mY, mSizeX, mSizeY);
}

void Dumb::Init()
{
}

void Dumb::Update()
{
	TILE[mIndexY][mIndexX]->SetObject(this);
}

void Dumb::Render(HDC hdc)
{
	if (mHp > 0 ) {
		CAMERA->RenderRect(hdc, mRect);
		if (mIsHit) {
			Gizmo::GetInstance()->DrawRect(hdc, mRect, Gizmo::Color::Red);
			mIsHit = false;
		}
			
	}
}

void Dumb::Damage(int a)
{
	if (mHp > 0) {
		mHp -= a;
		mIsHit = true;
	}
}