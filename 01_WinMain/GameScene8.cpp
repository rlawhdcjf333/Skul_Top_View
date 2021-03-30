#include "pch.h"
#include "GameScene8.h"
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

void GameScene8::Init()
{
	mRespawnCount = 4;
	MapLoad();
	GameObject* door = new Door(1220, 724);
	Obj->AddObject(ObjectLayer::Door, door);
	door->SetIsActive(false);
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(18, 44));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(18, 37));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(20, 43));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(20, 38));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_Wizard(30, 46));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_Wizard(30, 36));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_Wizard(24, 34));
	Obj->AddObject(ObjectLayer::Enemy, new Stage1_Wizard(24, 42));
	Obj->Init();

	Obj->AddObject(ObjectLayer::Player, SKUL->GetCurrentSkul());
	if (SKUL->GetAlterSkul())Obj->AddObject(ObjectLayer::Player, SKUL->GetAlterSkul());

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
			SceneManager::GetInstance()->LoadScene(L"GameScene9");
		}
	}

	if (mRespawnCount <= 0)
	{
		Obj->FindObject("Door")->SetIsActive(true);
	}

	if (Obj->GetObjectList(ObjectLayer::Enemy).size() == 0)
	{
		mRespawnCount--;
	}

	if (mRespawnCount == 3 && Obj->GetObjectList(ObjectLayer::Enemy).size() == 0)
	{
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(40, 44));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(39, 40));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(49, 36));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(43, 45));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hunter(42, 40));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hunter(41, 36));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hunter(45, 45));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hunter(45, 41));
	}
	if (mRespawnCount == 2 && Obj->GetObjectList(ObjectLayer::Enemy).size() == 0)
	{
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_NormalEnt(35, 62));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_NormalEnt(40, 66));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_NormalEnt(37, 60));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_NormalEnt(41, 63));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_NormalEnt(39, 58));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_NormalEnt(43, 60));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_NormalEnt(42, 55));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_NormalEnt(47, 57));

	}

	if (mRespawnCount == 1 && Obj->GetObjectList(ObjectLayer::Enemy).size() == 0)
	{
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(48, 43));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(57, 45));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hunter(49, 37));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Wizard(57, 38));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_SwordMan(52, 35));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hammer(57, 34));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Hunter(52, 31));
		Obj->AddObject(ObjectLayer::Enemy, new Stage1_Wizard(52, 25));
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
	mTileList.clear();

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