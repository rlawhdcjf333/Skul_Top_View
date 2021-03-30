#include "pch.h"
#include "GameScene9.h"
#include "Tile.h"
#include "MapObject.h"
#include "Door.h"
#include "Items.h"
#include "Animation.h"
#include "Boss.h"

void GameScene9::Init()
{
	SoundPlayer::GetInstance()->Stop(L"Stage2");
	SoundPlayer::GetInstance()->Stop(L"Main");
	SoundPlayer::GetInstance()->Play(L"Airman", 0.1);
	MapLoad();

	SKUL->GetCurrentSkul()->SetObjectOnTile(50, 48);
	
	GameObject* door = new Door(630, 900);
	door->SetIsActive(false);
	Obj->AddObject(ObjectLayer::Player, SKUL->GetCurrentSkul());
	Obj->AddObject(ObjectLayer::Boss, new Boss(40,40));
	Obj->AddObject(ObjectLayer::Door, door);
	Obj->Init();


	CAMERA->ChangeMode(Camera::Mode::Follow);
	CAMERA->SetTarget(SKUL->GetCurrentSkul());

	IMAGEMANAGER->LoadFromFile(L"back5", Resources(L"back5.bmp"), 1280, 740, false);
	mBack = IMAGEMANAGER->FindImage(L"back5");

	IMAGEMANAGER->LoadFromFile(L"ScoreFrame", Resources(L"ScoreFrame.bmp"), 1280, 720, false);
	mScoreFrame = IMAGEMANAGER->FindImage(L"ScoreFrame");

	mTogle = false;

	mDeathCount = 3;
	mKillCount = 192;
	mCurrentGold = SKUL->GetGold();
}

void GameScene9::Update()
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
			SceneManager::GetInstance()->LoadScene(L"GameScene");
		}
	}

	//if (mRespawnCount <= 0)
	//{
	//	Obj->FindObject("Door")->SetIsActive(true);
	//}
	Boss* boss = dynamic_cast<Boss*>(Obj->FindObject("Boss"));
	if (boss->GetDeadCheck()) {
		mDoorOpen = true;
	}

	if (INPUT->GetKeyDown('T'))
	{
		if (mTogle)mTogle = false;
		else if (!mTogle) mTogle = true;

		mPlayTime = (int)Time::GetInstance()->GetWorldTime();
		mScore = mCurrentGold + (2000 - ((mPlayTime / 60) * 100));
	}
}

void GameScene9::Render(HDC hdc)
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

	if (mTogle)
	{
		mScoreFrame->Render(hdc, 0, 0);

		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 535, 220, to_wstring(mPlayTime).c_str(), to_wstring(mPlayTime).size());
		TextOut(hdc, 535, 277, to_wstring(mDeathCount).c_str(), to_wstring(mDeathCount).size());
		TextOut(hdc, 530, 334, to_wstring(mKillCount).c_str(), to_wstring(mKillCount).size());
		TextOut(hdc, 535, 391, to_wstring(mCurrentGold).c_str(), to_wstring(mCurrentGold).size());
		TextOut(hdc, 525, 448, to_wstring(mScore).c_str(), to_wstring(mScore).size());
		SetBkMode(hdc, OPAQUE);
	}
}

void GameScene9::Release()
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

void GameScene9::MapLoad()
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