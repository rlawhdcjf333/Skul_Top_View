#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
	:mIsActive(true), mIsDestroy(false) {}

GameObject::GameObject(const string & name)
	: mName(name), mIsActive(true), mIsDestroy(false) {}

void GameObject::SetObjectOnTile(int indexX, int indexY)
{
	Tile* destination = TILE[indexY][indexX];
	mIndexX = indexX;
	mIndexY = indexY;
	mX = destination->GetCenterX();
	mY = destination->GetCenterY();

	destination->SetObject(this);
	destination->Update();
}
