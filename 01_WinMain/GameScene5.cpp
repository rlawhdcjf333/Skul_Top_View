#include "pch.h"
#include "GameScene5.h"
#include "Player.h"
#include "Tile.h"
#include "MapObject.h"
#include "Stage1_SwordMan.h"
#include "Door.h"
#include "Animation.h"

void GameScene5::Init()
{
	SoundPlayer::GetInstance()->Stop(L"Stage1");
	SoundPlayer::GetInstance()->Play(L"Store", 0.1);
	MapLoad();
	GameObject* door = new Door(1545, 972);
	Obj->AddObject(ObjectLayer::Door, door);
	Obj->Init();

	Obj->AddObject(ObjectLayer::Player, SKUL->GetCurrentSkul());
	if (SKUL->GetAlterSkul())Obj->AddObject(ObjectLayer::Player, SKUL->GetAlterSkul());

	SKUL->GetCurrentSkul()->SetObjectOnTile(22, 52);
	ITEM->RandomSpawn(36, 36);

	IMAGEMANAGER->LoadFromFile(L"back3", Resources(L"back3.bmp"), 1280, 740, false);
	mBack = IMAGEMANAGER->FindImage(L"back3");
	IMAGEMANAGER->LoadFromFile(L"StoreMaster", Resources(L"StoreMaster.bmp"), 1816, 151, 8, 1, true);
	mStoreMaster = IMAGEMANAGER->FindImage(L"StoreMaster");
	IMAGEMANAGER->LoadFromFile(L"StoreMaster2", Resources(L"StoreMaster2.bmp"), 1280, 720, 8, 1, true);
	mStoreMaster2 = IMAGEMANAGER->FindImage(L"StoreMaster2");

	mAnm = new Animation();
	mAnm->InitFrameByStartEnd(0, 0, 7, 0, false);
	mAnm->SetIsLoop(true);
	mAnm->SetFrameUpdateTime(0.1f);
	mAnm->Play();
	mtogle = false;
}

void GameScene5::Update()
{
	ObjectManager::GetInstance()->Update();
	mAnm->Update();
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

	RECT temp;
	RECT temp2 = Obj->FindObject("Door")->GetRect();
	RECT temp3 = SKUL->GetCurrentSkul()->GetRect();
	if (IntersectRect(&temp, &temp2, &temp3))
	{
		if (INPUT->GetKeyDown('F'))
		{
			SceneManager::GetInstance()->LoadScene(L"GameScene6");
		}
	}

	if (INPUT->GetKeyDown('T'))
	{
		if (mtogle == true)mtogle = false;

		else if (mtogle == false)mtogle = true;
	}

	if (INPUT->GetKeyDown('Y'))
	{
		if (SKUL->GetGold() >= 100)
		{
			SKUL->PlusHp(20);
			SKUL->PlusGold(-100);
		}
	}

	//if (INPUT->GetKeyDown('F'))
	//{
	//	SceneManager::GetInstance()->LoadScene(L"GameScene6");
	//}
}

void GameScene5::Render(HDC hdc)
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
	CAMERA->FrameRender(hdc, mStoreMaster, 535, 955,mAnm->GetNowFrameX(),0);
	if(mtogle==true)
	mStoreMaster2->Render(hdc, 0, 0);
}

void GameScene5::Release()
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
	SafeDelete(mAnm);
}

void GameScene5::MapLoad()
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

	ifstream loadStream(L"../04_Data/Stage1Map5/Tile.txt");
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

	loadStream.open(L"../04_Data/Stage1Map5/Object.txt");
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