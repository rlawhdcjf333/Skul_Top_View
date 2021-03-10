#pragma once

class ICommand
{
public:
	virtual void Execute() = 0;
	virtual void Undo() = 0;
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
	

public:
	IBrushHandle(Tile* tile, Pallete* pallete);
	void Execute()override;
	void Undo() override;
};