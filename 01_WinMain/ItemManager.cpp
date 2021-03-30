#include "pch.h"
#include "ItemManager.h"
#include "Items.h"

ItemManager::ItemManager()
{
	mAlreadySpawnedIndex = {};

	//스컬 머가리
	mItemContainer.insert(make_pair(0, [](int indexX, int indexY) {new SwordHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(1, [](int indexX, int indexY) {new PettyThiefHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(2, [](int indexX, int indexY) {new EntHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(3, [](int indexX, int indexY) {new WerewolfHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(4, [](int indexX, int indexY) {new AlchemistHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(5, [](int indexX, int indexY) {new ClownHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(6, [](int indexX, int indexY) {new HunterHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(7, [](int indexX, int indexY) {new MinoHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(8, [](int indexX, int indexY) {new BerserkerHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(9, [](int indexX, int indexY) {new WarriorHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(10, [](int indexX, int indexY) {new HighWarlockHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(11, [](int indexX, int indexY) {new NinjaHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(12, [](int indexX, int indexY) {new GrimReaperHead(indexX, indexY); }));
	mItemContainer.insert(make_pair(13, [](int indexX, int indexY) {new YakshaHead(indexX, indexY); }));

	//아이템
	mItemContainer.insert(make_pair(14, [](int indexX, int indexY) {new CaerleonSword(indexX, indexY); }));
	mItemContainer.insert(make_pair(15, [](int indexX, int indexY) {new DeterminationOfKnight(indexX, indexY); }));
	mItemContainer.insert(make_pair(16, [](int indexX, int indexY) {new BoneSandglass(indexX, indexY); }));
	mItemContainer.insert(make_pair(17, [](int indexX, int indexY) {new InfinityBone(indexX, indexY); }));
	mItemContainer.insert(make_pair(18, [](int indexX, int indexY) {new MagesNecklace(indexX, indexY); }));
	mItemContainer.insert(make_pair(19, [](int indexX, int indexY) {new ManaEater(indexX, indexY); }));
	mItemContainer.insert(make_pair(20, [](int indexX, int indexY) {new DimLightDarkcite(indexX, indexY); }));
	mItemContainer.insert(make_pair(21, [](int indexX, int indexY) {new HeartOfNaias(indexX, indexY); }));
	mItemContainer.insert(make_pair(22, [](int indexX, int indexY) {new HeartOfRuby(indexX, indexY); }));
	mItemContainer.insert(make_pair(23, [](int indexX, int indexY) {new AssassinationDagger(indexX, indexY); }));
	mItemContainer.insert(make_pair(24, [](int indexX, int indexY) {new BerserkersGlove(indexX, indexY); }));
	mItemContainer.insert(make_pair(25, [](int indexX, int indexY) {new BoneOfSpeed(indexX, indexY); }));
	mItemContainer.insert(make_pair(26, [](int indexX, int indexY) {new RageClaw(indexX, indexY); }));
	mItemContainer.insert(make_pair(27, [](int indexX, int indexY) {new BoneOfBrave(indexX, indexY); }));
	mItemContainer.insert(make_pair(28, [](int indexX, int indexY) {new GiantAxe(indexX, indexY); }));
	mItemContainer.insert(make_pair(29, [](int indexX, int indexY) {new HerosGlove(indexX, indexY); }));
	mItemContainer.insert(make_pair(30, [](int indexX, int indexY) {new GoldenIncenseBurner(indexX, indexY); }));
	mItemContainer.insert(make_pair(31, [](int indexX, int indexY) {new MedalOfCarleon(indexX, indexY); }));
	mItemContainer.insert(make_pair(32, [](int indexX, int indexY) {new LuckyStone(indexX, indexY); }));
	mItemContainer.insert(make_pair(33, [](int indexX, int indexY) {new BoneOfMana(indexX, indexY); }));
	mItemContainer.insert(make_pair(34, [](int indexX, int indexY) {new HatredStone(indexX, indexY); }));
	mItemContainer.insert(make_pair(35, [](int indexX, int indexY) {new RitualDagger(indexX, indexY); }));
	mItemContainer.insert(make_pair(36, [](int indexX, int indexY) {new RitualStaff(indexX, indexY); }));


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


