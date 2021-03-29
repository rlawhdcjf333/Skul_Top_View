#include "pch.h"
#include "GameScene8.h"
#include "Player.h"
#include "Tile.h"
#include "MapObject.h"
#include "Stage1_SwordMan.h"
#include "Door.h"

void GameScene8::Init()
{
	MapLoad();
	GameObject* door = new Door(1220, 724);
	Obj->AddObject(ObjectLayer::Door, door);
	door->SetIsActive(false);
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(16, 43));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(17, 39));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(19, 41));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(21, 38));
	Obj->Init();

	Obj->AddObject(ObjectLayer::Player, SKUL->GetCurrentSkul());
	Obj->AddObject(ObjectLayer::Player, SKUL->GetAlterSkul());

	SKUL->GetCurrentSkul()->SetObjectOnTile(3, 40);

	IMAGEMANAGER->LoadFromFile(L"back5", Resources(L"back5.bmp"), 1280, 740, false);
	mBack = IMAGEMANAGER->FindImage(L"back5");
}

void GameScene8::Update()
{
	ObjectManager::GetInstance()->Update();

	//}} 타일 클리핑
	RECT cameraRect = CAMERA->GetRect();
	float left = cameraRect.left;
	float top = cameraRect.top;
	x = (int)top / TileSizeY + (int)left / TileSizeX - (StartX / TileSizeX + StartY / TileSizeY);	//카메라 레프트 값에 대한 대략적인 인덱스값
	y = (int)top / TileSizeY - (int)left / TileSizeX + (StartX / TileSizeX - StartY / TileSizeY);
	offsetX = (int)left % TileSizeX;
	offsetY = (int)top % TileSizeY;

	if (offsetY < TileSizeY / 2 - offsetX / 2) { x--; }
	if (offsetY < offsetX / 2 - TileSizeY / 2) { y--; }
	if (offsetY > offsetX / 2 + TileSizeY / 2) { y++; }
	if (offsetY > 3 * TileSizeY / 2 - offsetX / 2) { x++; }
	//}}

	//if (INPUT->GetKeyDown(VK_CONTROL))
	//{
	//	Obj->GetObjectListPt(ObjectLayer::Enemy)->clear();
	//}

	if (mRespawnCount <= 0)
		Obj->FindObject("Door")->SetIsActive(true);

	if (Obj->GetObjectList(ObjectLayer::Enemy).size() == 0)
	{
		mRespawnCount--;
	}

	if (mRespawnCount == 3 && Obj->GetObjectList(ObjectLayer::Enemy).size() == 0)
	{
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(53, 26));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(55, 23));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(49, 18));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(48, 14));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(43, 13));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(44, 10));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(41, 7));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(38, 9));
	}
	if (mRespawnCount == 2 && Obj->GetObjectList(ObjectLayer::Enemy).size() == 0)
	{
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(41, 7));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(38, 9));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(36, 9));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(33, 15));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(28, 16));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(28, 24));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(29, 33));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(23, 36));

	}

	if (mRespawnCount == 1 && Obj->GetObjectList(ObjectLayer::Enemy).size() == 0)
	{
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(29, 33));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(23, 36));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(16, 42));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(17, 46));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(21, 51));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(29, 53));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(35, 50));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(37, 44));
	}
}

void GameScene8::Render(HDC hdc)
{
	mBack->Render(hdc, 0, 0);

	if (x >= 1 and x < 75 and y >= 1 and y < 75)
	{
		if (mTileList[y - 1][x - 1])
		{
			//일단 카메라기준으로 레프트탑 타일을 가져옴 융통성 있게 x좌표 y좌표 1 만큼 좀 뺌 (22,24)
			for (int j = 0; j < 26; j++)
			{
				for (int i = 0; i < 23; i++)
				{
					if (y - 1 - i + j < 0 || x - 1 + i + j>74 ||
						y - 1 - i + j > 74 || x - 1 + i + j < 0 ||
						y - 1 - i + j + 1 < 0 || y - 1 - i + j + 1 > 74)
						break;

					mTileList[y - 1 - i + j][x - 1 + i + j]->Render(hdc);
					mTileList[y - 1 - i + j + 1][x - 1 + i + j]->Render(hdc);
				}
			}
		}
	}


	ObjectManager::GetInstance()->Render(hdc);
}

void GameScene8::Release()
{
	for (auto elem : mTileList)
	{
		for (auto elemelem : elem)
		{
			SafeDelete(elemelem);
		}
	}
	SKUL->Reset();
	Obj->Release();

}

void GameScene8::MapLoad()
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

	ifstream loadStream(L"../04_Data/Stage1Map8/Tile.txt");
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
				int zaxis;
				string buffer;

				getline(loadStream, buffer, ',');
				key = buffer;
				getline(loadStream, buffer, ',');
				frameX = stoi(buffer);
				getline(loadStream, buffer, ',');
				frameY = stoi(buffer);
				getline(loadStream, buffer, ',');
				type = stoi(buffer);
				getline(loadStream, buffer);
				zaxis = stoi(buffer);

				wstring wstr;
				wstr.assign(key.begin(), key.end());
				mTileList[y][x]->SetImage(IMAGEMANAGER->FindImage(wstr));
				mTileList[y][x]->SetFrameX(frameX);
				mTileList[y][x]->SetFrameY(frameY);
				mTileList[y][x]->SetType((TileType)type);
				mTileList[y][x]->SetZaxis(zaxis);
			}
		}
	}
	loadStream.close();

	loadStream.open(L"../04_Data/Stage1Map8/Object.txt");
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