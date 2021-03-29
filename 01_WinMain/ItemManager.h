#pragma once

class ItemManager
{
	Singleton(ItemManager)

private:
	vector <int> mAlreadySpawnedIndex;
	map <int, function <void(int,int)>> mItemContainer;

public:

	ItemManager();

	//���ڷ� ���� Ÿ�� �ε��� ��ġ�� ���� �������� ��ȯ�ϴ� �Լ�. �ߺ� ������� ����. ����: ��� �Լ�
	void RandomSpawn(int indexX, int indexY);

};
#define ITEM ItemManager::GetInstance()