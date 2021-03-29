#pragma once

class ItemManager
{
	Singleton(ItemManager)

private:
	vector <int> mAlreadySpawnedIndex;
	map <int, function <void(int,int)>> mItemContainer;

public:

	ItemManager();

	//인자로 들어온 타일 인덱스 위치에 랜덤 아이템을 소환하는 함수. 중복 허용하지 않음. 원리: 재귀 함수
	void RandomSpawn(int indexX, int indexY);

};
#define ITEM ItemManager::GetInstance()