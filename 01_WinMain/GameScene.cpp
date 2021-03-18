#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Tile.h"
#include "Dumb.h"
#include "MapObject.h"
#include "Sword.h"

void GameScene::Init()
{
	IMAGEMANAGER->LoadFromFile(L"Tiles", Resources(L"tile_test.bmp"), 372, 372, 3, 6, true);
	IMAGEMANAGER->LoadFromFile(L"ForestObject", Resources(L"forestObject.bmp"), 360, 300, 3, 5, true);
	IMAGEMANAGER->LoadFromFile(L"Tree1", ResourcesObject(L"Tree1.bmp"), 200, 167, true);
	MapLoad();
	
	Obj->AddObject(ObjectLayer::Player, new Sword(30, 30, 30, 30));
	Obj->AddObject(ObjectLayer::Enemy,new Dumb());
	Obj->Init();

	CAMERA->ChangeMode(Camera::Mode::Follow);
	CAMERA->SetTarget(Obj->FindObject("player"));
}

void GameScene::Update()
{
	ObjectManager::GetInstance()->Update();
}

void GameScene::Render(HDC hdc)
{

	for (int y = 0; y < mTileList.size(); y++)
	{
		for (int x = 0; x < mTileList.size(); x++)
		{
			mTileList[y][x]->Render(hdc);
		}
	}

	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, 800, 100, L"이동: 우클릭, 꾹 누르고 있어도 됨.", 21);
	ObjectManager::GetInstance()->Render(hdc);
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

	Obj->Release();
	
}

void GameScene:: MapLoad()
{
	for (int y = 0; y < 75; y++)
	{
		vector <Tile*> tmp;
		for (int x = 0; x < 75; x++)
		{
			tmp.push_back
			(
				new Tile
				(
					IMAGEMANAGER->FindImage(L"Tiles"),
					StartX + (x - y) * TileSizeX / 2,
					StartY + (x + y) * TileSizeY / 2,
					2,
					3,
					TileSizeX,
					TileSizeY,
					x,
					y
				)
			);
		}
		mTileList.push_back(tmp);
	}

	ifstream loadStream(L"../04_Data/Tile.txt");
	if (loadStream.is_open())
	{
		for (int y = 0; y < mTileList.size(); ++y)
		{
			for (int x = 0; x < mTileList[y].size(); ++x)
			{
				string key;
				int frameX;
				int frameY;
				int type;
				string buffer;

				getline(loadStream, buffer, ',');
				key = buffer;
				getline(loadStream, buffer, ',');
				frameX = stoi(buffer);
				getline(loadStream, buffer, ',');
				frameY = stoi(buffer);
				getline(loadStream, buffer);
				type = stoi(buffer);

				wstring wstr;
				wstr.assign(key.begin(), key.end());
				mTileList[y][x]->SetImage(IMAGEMANAGER->FindImage(wstr));
				mTileList[y][x]->SetFrameX(frameX);
				mTileList[y][x]->SetFrameY(frameY);
				mTileList[y][x]->SetType((TileType)type);
			}
		}
	}
	loadStream.close();

	loadStream.open(L"../04_Data/Object.txt");
	if (loadStream.is_open())
	{
		while (loadStream.peek() != EOF) {
			string key;
			int x;
			int y;
			int type;
			string buffer;
			getline(loadStream, buffer, ',');
			key = buffer;
			getline(loadStream, buffer, ',');
			x = stoi(buffer);
			getline(loadStream, buffer);
			y = stoi(buffer);
			wstring wstr;
			wstr.assign(key.begin(), key.end());
			MapObject* mapObject = new MapObject(IMAGEMANAGER->FindImage(wstr), x, y);

			Obj->AddObject(ObjectLayer::MapObject, mapObject);
		}
	}

	TileList::GetInstance()->SetMap(mTileList);
}