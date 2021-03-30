#include "pch.h"
#include "ItemManager.h"
#include "Items.h"

ItemManager::ItemManager()
{
	mAlreadySpawnedIndex = {};


	mItemContainer.insert(make_pair(0, [](int indexX, int indexY) {new SwordHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(1, [](int indexX, int indexY) {new PettyThiefHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(2, [](int indexX, int indexY) {new EntHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(3, [](int indexX, int indexY) {new WerewolfHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(4, [](int indexX, int indexY) {new AlchemistHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(5, [](int indexX, int indexY) {new ClownHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(6, [](int indexX, int indexY) {new HunterHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(6, [](int indexX, int indexY) {new MinoHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(6, [](int indexX, int indexY) {new BerserkerHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(6, [](int indexX, int indexY) {new WarriorHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(6, [](int indexX, int indexY) {new HighWarlockHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(6, [](int indexX, int indexY) {new NinjaHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(6, [](int indexX, int indexY) {new GrimReaperHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(6, [](int indexX, int indexY) {new YakshaHead(indexX, indexY); }));


	mItemContainer.insert(make_pair(7, [](int indexX, int indexY) {new CaerleonSword(indexX, indexY); }));


}

void ItemManager::RandomSpawn(int indexX, int indexY)
{

	int index = RAND->RandomInt(mItemContainer.size());
	for (int elem : mAlreadySpawnedIndex)
	{
		if (elem == index)
		{
			RandomSpawn(indexX, indexY);
			return;
		}
	}
	mAlreadySpawnedIndex.push_back(index);
	mItemContainer[index](indexX, indexY);

}


