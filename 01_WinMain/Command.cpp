#include "pch.h"
#include "Command.h"
#include "Pallete.h"
#include "Tile.h"
#include "MapObject.h"
#include "MapObjectPallete.h"

IBrushHandle::IBrushHandle(Tile* tile, Pallete* pallete)
{
	mTarget = tile;
	mPallete = pallete;

	if (tile->GetImage() != nullptr)
	{
		mTargetKey = tile->GetImage()->GetKeyName();
	}
	mTargetFrameX = tile->GetFrameX();
	mTargetFrameY = tile->GetFrameY();
	mTargetType = tile->GetType();
}

void IBrushHandle::Execute()
{
	mTarget->SetImage(IMAGEMANAGER->FindImage(mPallete->GetImage()->GetKeyName()));
	mTarget->SetFrameX(mPallete->GetFrameX());
	mTarget->SetFrameY(mPallete->GetFrameY());
	mTarget->SetType(mPallete->GetType());
}

void IBrushHandle::Undo()
{
	mTarget->SetImage(IMAGEMANAGER->FindImage(mTargetKey));
	mTarget->SetFrameX(mTargetFrameX);
	mTarget->SetFrameY(mTargetFrameY);
	mTarget->SetType(mTargetType);
}
void IBrushHandle::Redo()
{
	//if (mRedoList.empty()) return;
	//mCommandList.emplace(mRedoList.top());
	mTarget->SetImage(IMAGEMANAGER->FindImage(mPallete->GetImage()->GetKeyName()));
	mTarget->SetFrameX(mPallete->GetFrameX());
	mTarget->SetFrameY(mPallete->GetFrameY());
	mTarget->SetType(mPallete->GetType());
}

IMapObject::IMapObject(POINT mousePosition, MapObjectPallete* pallete, vector<MapObject*>* list) {
	mPallete = pallete;
	mTargetlist = list;
	mSaveX = mousePosition.x;
	mSaveY = mousePosition.y;
	mObject = new MapObject(mPallete->GetImage(), mSaveX, mSaveY);
}

void IMapObject::Execute(){
	mTargetlist->push_back(mObject);
}

void IMapObject::Undo() 
{
	mTargetlist->pop_back();
}

void IMapObject::Redo()
{
	mTargetlist->push_back(mObject);
}
void IMapObject::Release()
{
	SafeDelete(mObject)
}