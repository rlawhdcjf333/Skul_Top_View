#pragma once
#include "Tile.h"

class ICommand
{
public:
	virtual void Execute() = 0;
	virtual void Undo() = 0;
	virtual void Redo() {}
	virtual void Release() {}
};

class Tile;
class Pallete;
class IBrushHandle :public ICommand
{
	Tile* mTarget;
	Pallete* mPallete;

	wstring mTargetKey;
	int mTargetFrameX;
	int mTargetFrameY;
	
	TileType mCurrentType;
	TileType mTargetType;

public:
	IBrushHandle(Tile* tile, Pallete* pallete);
	void Execute()override;
	void Undo() override;
	void Redo() override;
};

class MapObject;
class MapObjectPallete;
class IMapObject :public ICommand
{
	vector<MapObject*>* mTargetlist;
	float mSaveX;
	float mSaveY;
	MapObject* mObject;
	MapObjectPallete* mPallete;
	
public:
	IMapObject(POINT mousePoint, MapObjectPallete* pallete, vector<MapObject*>* list);
	void Execute()override;
	void Undo() override;
	void Redo() override;
	void Release() override;
};