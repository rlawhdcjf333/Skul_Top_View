#include "pch.h"
#include "Command.h"
#include "Pallete.h"
#include "Tile.h"

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
}

void IBrushHandle::Execute()
{
	mTarget->SetImage(IMAGEMANAGER->FindImage(mPallete->GetImage()->GetKeyName()));
	mTarget->SetFrameX(mPallete->GetFrameX());
	mTarget->SetFrameY(mPallete->GetFrameY());
}

void IBrushHandle::Undo()
{
	mTarget->SetImage(IMAGEMANAGER->FindImage(mTargetKey));
	mTarget->SetFrameX(mTargetFrameX);
	mTarget->SetFrameY(mTargetFrameY);

}
