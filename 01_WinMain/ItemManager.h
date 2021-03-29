#pragma once

class ItemManager
{

	map <wstring, function <void(int, int)>> mSkulHeadContainer;
	map <int, function <void(int,int)>> mItemContainer;



public:

	ItemManager();
	void RandomItemSpawn(int indexX, int indexY);
	void SkulHeadSpawn(wstring imageKeyName, int indexX, int indexY);

};