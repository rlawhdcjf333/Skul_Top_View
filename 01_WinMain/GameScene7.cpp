#include "pch.h"
#include "GameScene7.h"
#include "Player.h"
#include "Tile.h"
#include "MapObject.h"
#include "Stage1_SwordMan.h"
#include "Door.h"
#include "Stage1_SwordMan.h"
#include "Stage1_Hammer.h"
#include "Stage1_Hunter.h"
#include "Stage1_NormalEnt.h"
#include "Stage1_Wizard.h"

void GameScene7::Init()
{
	mRespawnCount = 4;
	MapLoad();
	GameObject* door = new Door(1690, 985);
	Obj->AddObject(ObjectLayer::Door, door);
	door->SetIsActive(false);
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_NormalEnt(24, 44));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(30, 50));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_NormalEnt(22, 38));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(42, 51));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_NormalEnt(31, 41));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_Wizard(37, 59));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_Wizard(10, 39));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_Wizard(24, 32));
	Obj->Init();

	Obj->AddObject(ObjectLayer::Player, SKUL->GetCurrentSkul());
	if (SKUL->GetAlterSkul())Obj->AddObject(ObjectLayer::Player, SKUL->GetAlterSkul());

	SKUL->GetCurrentSkul()->SetObjectOnTile(23, 50);

	IMAGEMANAGER->LoadFromFile(L"back5", Resources(L"back5.bmp"), 1280, 740, false);
	mBack = IMAGEMANAGER->FindImage(L"back5");
}

void GameScene7::Update()
{
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
	Door* door = (Door*)Obj->FindObject(ObjectLayer::Door, "Door");
	if (mDoorOpen && !door->DoorOpenCheck()) {
		if (mDoorEventTime > 0) {
			mDoorEventTime -= dTime;
			CAMERA->ChangeMode(Camera::Mode::Follow);
			CAMERA->SetTarget(door);
		}
		else {
			if (!door->GetIsActive()) {
				ITEM->RandomSpawn(50, 23);
				SKUL->PlusGold(RAND->RandomInt(80, 130));
				SKUL->PlusHp(20);
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
			SceneManager::GetInstance()->LoadScene(L"GameScene8");
		}
	}

	//if (INPUT->GetKeyDown('F'))
	//{
	//	SceneManager::GetInstance()->LoadScene(L"GameScene8");
	//}

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
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_NormalEnt(10, 39));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_NormalEnt(24, 32));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(49, 18));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(48, 14));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(43, 13));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hunter(44, 10));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hunter(41, 7));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Wizard(38, 9));
	}
	if (mRespawnCount == 2 && Obj->GetObjectList(ObjectLayer::Enemy).size() == 0)
	{
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(41, 7));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(38, 9));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(36, 9));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(33, 15));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hunter(34, 16));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hunter(35, 17));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Wizard(29, 33));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Wizard(23, 36));

	}

	if (mRespawnCount == 1 && Obj->GetObjectList(ObjectLayer::Enemy).size() == 0)
	{
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(29, 33));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(23, 36));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(16, 42));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hunter(35, 50));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Wizard(21, 51));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hunter(29, 53));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Wizard(35, 50));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hunter(37, 44));
	}


}

void GameScene7::Render(HDC hdc)
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

void GameScene7::Release()
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

void GameScene7::MapLoad()
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

	ifstream loadStream(L"../04_Data/Stage1Map7/Tile.txt");
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

	loadStream.open(L"../04_Data/Stage1Map7/Object.txt");
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