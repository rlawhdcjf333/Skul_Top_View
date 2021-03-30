#include "pch.h"
#include "GameScene3.h"
#include "Player.h"
#include "Tile.h"
#include "MapObject.h"
#include "Stage1_SwordMan.h"
#include "Door.h"

void GameScene3::Init()
{
	mRespawnCount = 4;
	MapLoad();
	GameObject* door = new Door(1627, 743);
	Obj->AddObject(ObjectLayer::Door, door);
	door->SetIsActive(false);
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(26, 34));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(29, 42));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(30, 38));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(35, 41));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(25, 45));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(25, 32));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(28, 51));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(29, 29));
	Obj->Init();

	Obj->AddObject(ObjectLayer::Player, SKUL->GetCurrentSkul());
	if (SKUL->GetAlterSkul())Obj->AddObject(ObjectLayer::Player, SKUL->GetAlterSkul());

	SKUL->GetCurrentSkul()->SetObjectOnTile(18, 39);

	IMAGEMANAGER->LoadFromFile(L"back2", Resources(L"back2.bmp"), 1280, 740, false);
	mBack = IMAGEMANAGER->FindImage(L"back2");
}

void GameScene3::Update()
{
	//}} Ÿ�� Ŭ����
	RECT cameraRect = CAMERA->GetRect();
	float left = cameraRect.left;
	float top = cameraRect.top;
	x = (int)top / TileSizeY + (int)left / TileSizeX - (StartX / TileSizeX + StartY / TileSizeY);	//ī�޶� ����Ʈ ���� ���� �뷫���� �ε�����
	y = (int)top / TileSizeY - (int)left / TileSizeX + (StartX / TileSizeX - StartY / TileSizeY);
	offsetX = (int)left % TileSizeX;
	offsetY = (int)top % TileSizeY;

	if (offsetY < TileSizeY / 2 - offsetX / 2) { x--; }
	if (offsetY < offsetX / 2 - TileSizeY / 2) { y--; }
	if (offsetY > offsetX / 2 + TileSizeY / 2) { y++; }
	if (offsetY > 3 * TileSizeY / 2 - offsetX / 2) { x++; }
	//}}
	Door* door = (Door*)Obj->FindObject(ObjectLayer::Door, "Door");
	if (mDoorOpen && !door->DoorOpenCheck()) {
		if (mDoorEventTime > 0) {
			mDoorEventTime -= dTime;
			CAMERA->ChangeMode(Camera::Mode::Follow);
			CAMERA->SetTarget(door);
		}
		else {
			if (!door->GetIsActive()) {
				ITEM->RandomSpawn(45, 20);
				door->SetIsActive(true);
			}
			door->Update();

		}
		CAMERA->PanningOn(2);
		CAMERA->Panning();
		return;
	}
	else if (mOpenTime > 0 && door->DoorOpenCheck()) {
		mOpenTime -= dTime;
		door->Update();
		return;
	}
	else {
		CAMERA->SetTarget(SKUL->GetCurrentSkul());
	}
	ObjectManager::GetInstance()->Update();
	RECT temp;
	RECT temp2 = Obj->FindObject("Door")->GetRect();
	RECT temp3 = SKUL->GetCurrentSkul()->GetHitBox();
	if (IntersectRect(&temp, &temp2, &temp3))
	{
		if (INPUT->GetKeyDown('F'))
		{
			SceneManager::GetInstance()->LoadScene(L"GameScene4");
		}
	}

	//if (INPUT->GetKeyDown(VK_CONTROL))
	//{
	//	Obj->GetObjectListPt(ObjectLayer::Enemy)->clear();
	//}

	if (mRespawnCount <= 0)
		mDoorOpen = true;

	if (Obj->GetObjectList(ObjectLayer::Enemy).size() == 0)
	{
		mRespawnCount--;
	}

	if (mRespawnCount == 3 && Obj->GetObjectList(ObjectLayer::Enemy).size() == 0)
	{
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(28, 35));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(26, 44));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(37, 35));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(35, 46));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(41, 44));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(42, 38));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(45, 44));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(44, 29));
	}
	if (mRespawnCount == 2 && Obj->GetObjectList(ObjectLayer::Enemy).size() == 0)
	{
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(45, 44));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(44, 29));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(42, 46));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(48, 52));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(53, 50));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(53, 44));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(58, 43));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(53, 36));

	}

	if (mRespawnCount == 1 && Obj->GetObjectList(ObjectLayer::Enemy).size() == 0)
	{
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(58, 43));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(53, 36));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(61, 39));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(60, 34));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(57, 28));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(52, 27));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(46, 31));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(46, 27));
	}

	//if (INPUT->GetKeyDown('F'))
	//{
	//	SceneManager::GetInstance()->LoadScene(L"GameScene4");
	//}
}

void GameScene3::Render(HDC hdc)
{
	mBack->Render(hdc, 0, 0);

	if (x >= 1 and x < 75 and y >= 1 and y < 75)
	{
		if (mTileList[y - 1][x - 1])
		{
			//�ϴ� ī�޶�������� ����Ʈž Ÿ���� ������ ���뼺 �ְ� x��ǥ y��ǥ 1 ��ŭ �� �� (22,24)
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

void GameScene3::Release()
{
	for (auto elem : mTileList)
	{
		for (auto elemelem : elem)
		{
			SafeDelete(elemelem);
		}
	}
	mTileList.clear();

	SKUL->Reset();
	Obj->Release();

}

void GameScene3::MapLoad()
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

	ifstream loadStream(L"../04_Data/Stage1Map3/Tile.txt");
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

	loadStream.open(L"../04_Data/Stage1Map3/Object.txt");
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