#include "pch.h"
#include "GameScene.h"
#include "Player.h"
#include "Tile.h"
#include "Dumb.h"
#include "MapObject.h"

void GameScene::Init()
{
	MapLoad();
	GameObject* little = new LittleBone(30, 30, 30, 30);
	GameObject* mino = new Clown(30, 30, 30, 30);
	mino->SetIsActive(false);
	Obj->AddObject(ObjectLayer::Player, little);
	Obj->AddObject(ObjectLayer::Player, mino);
	SKUL->SetCurrentSkul((Player*)mino);
	SKUL->NewSkulGet((Player*)little);
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

	//대충 최적화
	for (int y = 0; y <mTileList.size(); y++)
	{
		for (int x = 0; x < mTileList.size(); x++)
		{
			if (x + y > 39 and x + y<110 and y - x>-42 and y - x < 42)
			{
				mTileList[y][x]->Render(hdc);
			}
			else if (y - x <= -42 or y+x>=110)
			{
				break;
			}
			else if ( y + x <= 39)
			{
				x = 39 - y;
			}
			else if (y - x >= 42)
			{
				x = y - 42;
			}
		}
	}

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

	ifstream loadStream(L"../04_Data/Stage1Map1/Tile.txt");
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

	loadStream.open(L"../04_Data/Stage1Map1/Object.txt");
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