#include "pch.h"
#include "TileSelect.h"
#include "Camera.h"
void TileSelect::Init()
{
}

void TileSelect::Release()
{
}

void TileSelect::Update()
{
	mCurrentTile = nullptr;
	LONG _mousePositionX = CAMERA->CameraMouseX();
	LONG _mousePositionY = CAMERA->CameraMouseY();
	int x = _mousePositionY / TileSizeY + _mousePositionX / TileSizeX - (StartX / TileSizeX + StartY / TileSizeY);
	int y = _mousePositionY / TileSizeY - _mousePositionX / TileSizeX + (StartX / TileSizeX - StartY / TileSizeY);
	int offsetX = _mousePositionX % TileSizeX;
	int offsetY = _mousePositionY % TileSizeY;

	if (offsetY < TileSizeY / 2 - offsetX / 2) { x--; }
	if (offsetY < offsetX / 2 - TileSizeY / 2) { y--; }
	if (offsetY > offsetX / 2 + TileSizeY / 2) { y++; }
	if (offsetY > 3 * TileSizeY / 2 - offsetX / 2) { x++; }

	if (x >= 0 and x < TILESizeX  and y >= 0 and y < TILESizeY)
	{
		mCurrentTile = TILE[y][x];
	}
}

void TileSelect::Render(HDC hdc)
{
	if (mCurrentTile)
	{
		mCurrentTile->SelectRender(hdc);
	}
}
