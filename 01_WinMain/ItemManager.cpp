#include "pch.h"
#include "ItemManager.h"
#include "Items.h"

ItemManager::ItemManager()
{
	mSkulHeadContainer.insert(make_pair (L"LittleBone", [](int indexX, int indexY) {new LittleBoneHead(indexX, indexY);}));





	mItemContainer.insert(make_pair(0, [](int indexX, int indexY) {new LittleBoneHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(1, [](int indexX, int indexY) {new CaerleonSword(indexX, indexY); }));


}

void ItemManager::RandomItemSpawn(int indexX, int indexY)
{
	mItemContainer[RAND->RandomInt(mItemContainer.size())](indexX, indexY);
}

void ItemManager::SkulHeadSpawn(wstring imageKeyName, int indexX, int indexY)
{
	mSkulHeadContainer.at(imageKeyName)(indexX, indexY);
}

