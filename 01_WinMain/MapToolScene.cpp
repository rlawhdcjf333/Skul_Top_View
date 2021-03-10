#include "pch.h"
#include "MapToolScene.h"
#include "Button.h"
#include "Tile.h"
#include "Pallete.h"
#include "Command.h"

void MapToolScene::RegisterCommand(ICommand* command)
{
	command->Execute();
	mCommandList.emplace(command);
}

void MapToolScene::Save()
{
	ofstream saveStream(L"../04_Data/Test.txt");
	if (saveStream.is_open())
	{
		string tempImageKey;
		int frameX;
		int frameY;

		for (int y = 0; y < mTileList.size(); ++y)
		{
			for (int x = 0; x < mTileList[y].size(); ++x)
			{
				string str;
				wstring keyName;
				if (mTileList[y][x]->GetImage() != nullptr)
					keyName = mTileList[y][x]->GetImage()->GetKeyName();
				str.assign(keyName.begin(), keyName.end());

				saveStream << str.c_str();
				saveStream << ',';
				saveStream << mTileList[y][x]->GetFrameX();
				saveStream << ',';
				saveStream << mTileList[y][x]->GetFrameY();
				saveStream << endl;
			}
		}
	}
}

void MapToolScene::Load()
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

				getline(loadStream, buffer,',');
				key = buffer;
				getline(loadStream, buffer,',');
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

void MapToolScene::Undo()
{
	if (mCommandList.empty()) return;

	mRedoList.emplace(mCommandList.top());
	mCommandList.top()->Undo();
	mCommandList.pop();
}

void MapToolScene::Redo()
{
	if (mRedoList.empty()) return;
	
	mCommandList.emplace(mRedoList.top());
	mRedoList.top()->Execute();
	mRedoList.pop();
}

void MapToolScene::Init()
{
	IMAGEMANAGER->LoadFromFile(L"Tiles", Resources(L"Tiles.bmp"), 372, 372, 3,6, true);
	mImage = IMAGEMANAGER->FindImage(L"Tiles");

	for (int y = 0; y < 20; y++)
	{
		vector <Tile*> tmp;
		for (int x = 0; x < 20; x++)
		{
			//회전 축변환... 생각해보면 별거없다 ㄹㅇ
			tmp.push_back
			( 
				new Tile
				(
				mImage,
				StartX+(x-y)*TileSizeX/2,
				StartY+(x+y)*TileSizeY/2, 
				2,
				3,
				TileSizeX,
				TileSizeY
				)
			);
		}
		mTileList.push_back(tmp);
	}


	for (int y = 0; y < 3; y++)
	{
		vector <Pallete*> tmp;
		for (int x = 0; x < 5; x++)
		{
			tmp.push_back(
				new Pallete
				(
					mImage,
					900 + 50 * x,
					100 + 25 * y,
					y,
					x,
					50,
					25
				)
			);
		}
		mPalleteList.push_back(tmp);
	}


	mSave = new Button(L"저장", 150, 25, 200, 50, [this]() {Save();});
	mLoad = new Button(L"불러오기", 360, 25, 200, 50, [this]() {Load();});
	mUndo = new Button(L"되돌리기", 570, 25, 200, 50, [this]() {Undo();});
	mRedo = new Button(L"되돌리기 취소", 780, 25, 200, 50, [this]() {Redo();});
	mNext = new Button(L"게임로드", 990, 25, 200, 50, [this]() {SceneManager::GetInstance()->LoadScene(L"GameScene");});

}

void MapToolScene::Release()
{

	for (int y = 0; y < mTileList.size(); y++)
	{
		for (int x = 0; x < mTileList[y].size(); x++)
		{
			SafeDelete(mTileList[y][x]);
		}
	}

	for (int y = 0; y < mPalleteList.size(); y++)
	{
		for (int x = 0; x < mPalleteList[y].size(); x++)
		{
			SafeDelete(mPalleteList[y][x]);
		}
	}

	while (!mCommandList.empty())
	{
		auto iter = mCommandList.top();
		SafeDelete(iter);
		mCommandList.pop();
	}

	while (!mRedoList.empty())
	{
		auto iter = mRedoList.top();
		SafeDelete(iter);
		mRedoList.pop();
	}

	SafeDelete(mSave);
	SafeDelete(mLoad);
	SafeDelete(mUndo);
	SafeDelete(mRedo);
	SafeDelete(mNext);
}

void MapToolScene::Update()
{
	mCurrentTile = nullptr;
	//start 벡터를 빼서 정렬해야하는데 이게 나머지 연산 몫이 잘 안떨어지면 따로 계산해야 정확하다...
	int x = _mousePosition.y/ TileSizeY + _mousePosition.x / TileSizeX -13;
	int y = _mousePosition.y/ TileSizeY - _mousePosition.x / TileSizeX +7;
	int offsetX = _mousePosition.x % TileSizeX;
	int offsetY = _mousePosition.y % TileSizeY;

	//ㅅㅂ  어떤 ㅅㄲ가 멍청하게 윈도우 y축 뒤집어서 만들었냐 ㄹㅇ
	if (offsetY < TileSizeY / 2 - offsetX / 2) { x--; }
	if (offsetY < offsetX / 2 - TileSizeY / 2) { y--; }
	if (offsetY > offsetX / 2 + TileSizeY / 2) { y++; }
	if (offsetY > 3 * TileSizeY / 2 - offsetX / 2) { x++; }

	if (x >= 0 and x < 20 and y >= 0 and y < 20)
	{
		mCurrentTile = mTileList[y][x];
	}


	for (auto elem : mPalleteList)
	{
		for (auto elemelem : elem)
		{
			if (PtInRect(elemelem->GetRect(), _mousePosition) and Input::GetInstance()->GetKeyUp(VK_LBUTTON))
			{
				mCurrentPallete = elemelem;
			}
		}
	}

	if (mCurrentTile and mCurrentPallete)
	{
		if (Input::GetInstance()->GetKey(VK_LBUTTON))
		{
			if (mCurrentTile->GetImage() != mCurrentPallete->GetImage() or
				mCurrentTile->GetFrameX() != mCurrentPallete->GetFrameX() or
				mCurrentTile ->GetFrameY() != mCurrentPallete->GetFrameY())
			{
				IBrushHandle* command = new IBrushHandle(mCurrentTile, mCurrentPallete);
				RegisterCommand(command);
			}
		}
	}


	mSave->Update();
	mLoad->Update();
	mUndo->Update();
	mRedo->Update();
	mNext->Update();
}

void MapToolScene::Render(HDC hdc)
{
	for (int y = 0; y < mTileList.size(); y++)
	{
		for (int x = 0; x < mTileList[y].size(); x++)
		{
			mTileList[y][x]->Render(hdc);
		}
	}

	if (mCurrentTile)
	{
		mCurrentTile->SelectRender(hdc);
	}

	mSave->Render(hdc);
	mLoad->Render(hdc);
	mUndo->Render(hdc);
	mRedo->Render(hdc);
	mNext->Render(hdc);

	for (int y = 0; y < mPalleteList.size(); y++)
	{
		for (int x = 0; x< mPalleteList[y].size();x++)
		{
			mPalleteList[y][x]->Render(hdc);
		}
	}

	if (mCurrentPallete)
	{
		mCurrentPallete->Render(hdc, _mousePosition.x - 25, _mousePosition.y - 12);
	}

}
