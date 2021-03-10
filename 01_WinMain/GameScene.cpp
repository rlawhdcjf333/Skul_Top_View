#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Tile.h"

void GameScene::Init()
{
	for (int y = 0; y < 20; y++)
	{
		vector <Tile*> tmp;
		for (int x = 0; x < 20; x++)
		{
			tmp.push_back
			(
				new Tile
				(
					IMAGEMANAGER->FindImage(L"Tiles"),
					610 + (x - y) * 60 / 2,
					100 + (x + y) * 30 / 2,
					2,
					3,
					60,
					30
				)
			);
		}
		mTileList.push_back(tmp);
	}
	MapLoad();
	mPlayer = new Player(mTileList[10][10]->GetX()+30, mTileList[10][10]->GetY()+15, 30,30);
	mPlayer->Init();
}

void GameScene::Update()
{
	mPlayer->Update();
}

void GameScene::Render(HDC hdc)
{

	for (int y = 0; y < mTileList.size(); y++)
	{
		for (int x = 0; x < mTileList[y].size(); x++)
		{
			mTileList[y][x]->Render(hdc);
		}
	}

	mPlayer->Render(hdc);

	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, 800, 100, L"이동: WASD", 8);
	TextOut(hdc, 850, 150, L"타일맵 enum class 구현은 나중에", 22);
}

void GameScene::Release()
{
	for (auto elem : mTileList)
	{
		for (auto elemelem : elem)
		{
			SafeDelete(elemelem);
		}
	}


	SafeDelete(mPlayer);
}

void GameScene:: MapLoad()
{
	ifstream loadStream(L"../04_Data/Test.txt");
	if (loadStream.is_open())
	{
		for (int y = 0; y < mTileList.size(); ++y)
		{
			for (int x = 0; x < mTileList[y].size(); ++x)
			{
				string key;
				int frameX;
				int frameY;
				string buffer;

				getline(loadStream, buffer, ',');
				key = buffer;
				getline(loadStream, buffer, ',');
				frameX = stoi(buffer);
				getline(loadStream, buffer);
				frameY = stoi(buffer);

				wstring wstr;
				wstr.assign(key.begin(), key.end());
				mTileList[y][x]->SetImage(IMAGEMANAGER->FindImage(wstr));
				mTileList[y][x]->SetFrameX(frameX);
				mTileList[y][x]->SetFrameY(frameY);
			}
		}
	}
}