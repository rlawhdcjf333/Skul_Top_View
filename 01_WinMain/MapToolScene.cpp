#include "pch.h"
#include "MapToolScene.h"
#include "Button.h"
#include "Command.h"
#include "Tile.h"
#include "Pallete.h"
#include "MapObject.h"
#include "MapObjectPallete.h"

void MapToolScene::RegisterCommand(ICommand* command)
{
	command->Execute();
	if (mCurrentLayer==Layer::Object) {
		mObjectCommandList.emplace(command);
	}
	else if (mCurrentLayer==Layer::Tile) {
		mCommandList.emplace(command);
	}
}

void MapToolScene::Save()
{
	ofstream saveStream(L"../04_Data/Tile.txt");
	if (saveStream.is_open())
	{
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
				saveStream << ',';
				saveStream << (int)mTileList[y][x]->GetType();
				saveStream << endl;
			}
		}
	}
	saveStream.close();
	saveStream.open(L"../04_Data/Object.txt");
	if (saveStream.is_open())
	{
		for ( MapObject* elem: mMapObjectList)
		{
			string str;
			wstring keyName;
			if (elem->GetImageKey() != L"")
				keyName = elem->GetImageKey();
			str.assign(keyName.begin(), keyName.end());

			saveStream << str.c_str();
			saveStream << ',';
			saveStream << elem->GetX();
			saveStream << ',';
			saveStream << elem->GetY();
			saveStream << endl;
		}
	}
}

void MapToolScene::Load()
{
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

				getline(loadStream, buffer,',');
				key = buffer;
				getline(loadStream, buffer,',');
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
	loadStream.close(); //위에 열러있는 txt 파일을 닫아 연결을 해제한다.
	//다른 파일을 저장 읽을 시 하나의 파일만 이용 가능
	//원래 open을 하면 자동으로 닫혀야 하지만 이상하게 닫히지 않아서 close를 써서 강제로 해제 중
	loadStream.open(L"../04_Data/Object.txt");
	if (loadStream.is_open()) //파일을 정상적으로 열었는지 확인
	{
		vector <MapObject*> tmp;
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
			tmp.emplace_back(mapObject);
		 }

		mMapObjectList.assign(tmp.begin(), tmp.end());
	}
}

void MapToolScene::Undo()
{
	if (mCurrentLayer == Layer::Object) {
		if (mObjectCommandList.empty()) return;

		mObjectRedoList.emplace(mObjectCommandList.top());
		mObjectCommandList.top()->Undo();
		mObjectCommandList.pop();
	}
	else if (mCurrentLayer == Layer::Tile) {
		if (mCommandList.empty()) return;

		mRedoList.emplace(mCommandList.top());
		mCommandList.top()->Undo();
		mCommandList.pop();
	}
}

void MapToolScene::Redo()
{
	if (mCurrentLayer == Layer::Object) {
		if (mObjectRedoList.empty()) return;

		mObjectCommandList.emplace(mObjectRedoList.top());
		mObjectRedoList.top()->Redo();
		mObjectRedoList.pop();
	}
	else if (mCurrentLayer == Layer::Tile) {
		if (mRedoList.empty()) return;

		mCommandList.emplace(mRedoList.top());
		mRedoList.top()->Redo();
		mRedoList.pop();
	}
}

void MapToolScene::ChangeLayer()
{
	mCurrentLayer = Layer((int)mCurrentLayer + 1);
	if (mCurrentLayer == Layer::End)
	{
		mCurrentLayer = Layer::Tile;
	}
}

void MapToolScene::stackClear(stack<ICommand*>* stack)
{
	while (!stack->empty()) {
		stack->pop();
	}
}

void MapToolScene::Init()
{
	IMAGEMANAGER->LoadFromFile(L"Tiles", Resources(L"tile_test.bmp"), 372, 372, 3,6, true);
	IMAGEMANAGER->LoadFromFile(L"ForestObject", Resources(L"forestObject.bmp"), 360, 300, 3, 5, true);
	IMAGEMANAGER->LoadFromFile(L"Tree1", ResourcesObject(L"Tree1.bmp"),200,167,true);
	mImage = IMAGEMANAGER->FindImage(L"Tiles");

	for (int y = 0; y < 75; y++) // 타일 레이어 도화지
	{
		vector <Tile*> tmp;
		for (int x = 0; x < 75; x++)
		{
			tmp.push_back
			( 
				new Tile
				(
				nullptr,
				StartX+(x-y)*TileSizeX/2,
				StartY+(x+y)*TileSizeY/2, 
				0,
				0,
				TileSizeX,
				TileSizeY,
				x,
				y
				)
			);
		}
		mTileList.push_back(tmp);
	}
	for (int y = 0; y < 3; y++) //타일 팔레트
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
	vector <MapObjectPallete*> tmp;
	tmp.push_back(new MapObjectPallete(IMAGEMANAGER->FindImage(L"Tree1"), 900, 100));
	mMapObjectPallete.push_back(tmp);

	mPalletRc = RectMake(900, 100, 5 * 50, 3 * 25);
	mSave = new Button(L"저장", 150, 25, 200, 50, [this]() {Save();});
	mLoad = new Button(L"불러오기", 360, 25, 200, 50, [this]() {Load();});
	mUndo = new Button(L"되돌리기", 570, 25, 200, 50, [this]() {Undo();});
	mRedo = new Button(L"되돌리기 취소", 780, 25, 200, 50, [this]() {Redo();});
	mNext = new Button(L"게임로드", 990, 25, 200, 50, []() {SceneManager::GetInstance()->LoadScene(L"GameScene");});
	mChangeLayer = new Button(L"레이어 변경", 150, 80, 200, 50, [this]() {ChangeLayer(); });
	mMenuRc = RectMake(150, 25, 1190, 150);
	CameraManager::GetInstance()->GetMainCamera()->ChangeMode(Camera::Mode::Free);
	CAMERA->SetX(StartX);
	CAMERA->SetY(StartY);
	CAMERA->SetMoveSpeed(20.f);
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
	for (MapObject* elem : mMapObjectList)
	{
		SafeDelete(elem);
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
	SafeDelete(mChangeLayer);
	SafeDelete(mNext);
}

void MapToolScene::Update()
{
	mCurrentTile = nullptr;

	int x = CAMERA->CameraMouseY() / TileSizeY + CAMERA->CameraMouseX() / TileSizeX - (StartX / TileSizeX + StartY / TileSizeY);
	int y = CAMERA->CameraMouseY() / TileSizeY - CAMERA->CameraMouseX() / TileSizeX + (StartX / TileSizeX - StartY / TileSizeY);
	int offsetX = CAMERA->CameraMouseX() % TileSizeX;
	int offsetY = CAMERA->CameraMouseY() % TileSizeY;


	if (offsetY < TileSizeY / 2 - offsetX / 2) { x--; }
	if (offsetY < offsetX / 2 - TileSizeY / 2) { y--; }
	if (offsetY > offsetX / 2 + TileSizeY / 2) { y++; }
	if (offsetY > 3 * TileSizeY / 2 - offsetX / 2) { x++; }
	
	if (x >= 0 and x < mTileList.size() and y >= 0 and y < mTileList.size())
	{
		mCurrentTile = mTileList[y][x];
	}

	if (INPUT->GetKeyDown(VK_TAB)) {
		mTabKey = !mTabKey;
	}
	if (INPUT->GetKeyDown(VK_ESCAPE)) {
		mRenderToggle = !mRenderToggle;
	}
	if (mCurrentPallete and INPUT->GetKeyDown(VK_RBUTTON))
	{
		mCurrentPallete->SetType(TileType((int)mCurrentPallete->GetType() + 1));
		if (mCurrentPallete->GetType() == TileType::End)
		{
			mCurrentPallete->SetType(TileType::Normal);
		}
	}
	if (mCurrentTile and mCurrentPallete)
	{
		if (!PtInRect(&mPalletRc, nonC_mousePosition) and !PtInRect(&mMenuRc, nonC_mousePosition) and Input::GetInstance()->GetKey(VK_LBUTTON))
		{
			if (mCurrentTile->GetImage() != mCurrentPallete->GetImage() or
				mCurrentTile->GetFrameX() != mCurrentPallete->GetFrameX() or
				mCurrentTile->GetFrameY() != mCurrentPallete->GetFrameY() or
				mCurrentTile->GetType() != mCurrentPallete->GetType())
			{
				IBrushHandle* command = new IBrushHandle(mCurrentTile, mCurrentPallete);
				RegisterCommand(command);
			}
		}
	}
	if (mCurrentMapObjectPallete)
	{
		if (!PtInRect(&mPalletRc, nonC_mousePosition) and !PtInRect(&mMenuRc, nonC_mousePosition) and Input::GetInstance()->GetKeyUp(VK_LBUTTON))
		{
			IMapObject* command = new IMapObject(_mousePosition, mCurrentMapObjectPallete,&mMapObjectList);
			RegisterCommand(command);
		}
	}
	if (!mTabKey) {
		if (mCurrentLayer == Layer::Tile)
		{
			for (auto elem : mPalleteList)
			{
				for (auto elemelem : elem)
				{
					if (PtInRect(elemelem->GetRect(), nonC_mousePosition) and Input::GetInstance()->GetKeyUp(VK_LBUTTON))
					{
						mCurrentPallete = elemelem;
					}
				}
			}
		}
		else if (mCurrentLayer == Layer::Object)
		{
			for (auto elem : mMapObjectPallete)
			{
				for (auto elemelem : elem)
				{
					RECT rc = elemelem->GetRect();
					if (PtInRect(&rc, nonC_mousePosition) and Input::GetInstance()->GetKeyUp(VK_LBUTTON))
					{
						mCurrentMapObjectPallete = elemelem;
					}
				}
			}

		
		}

		function <bool(MapObject* a, MapObject* b)> comp = [](MapObject* a, MapObject* b) 
		{ 
			return a->GetY() < b->GetY();
		};

		sort(mMapObjectList.begin(), mMapObjectList.end(), comp);

		mSave->Update();
		mLoad->Update();
		mUndo->Update();
		mRedo->Update();
		mChangeLayer->Update();
		mNext->Update();

	}
}

void MapToolScene::Render(HDC hdc)
{
	for (int y = 0; y < mTileList.size(); y++)
	{
		for (int x = 0; x < mTileList[y].size(); x++)
		{
			if (mCurrentLayer == Layer::Tile) {
				mTileList[y][x]->Render(hdc);
			}
			else {
				mTileList[y][x]->AlphaRender(hdc);
			}
			if (!mRenderToggle) {
				mTileList[y][x]->SelectRenderMargenta(hdc);
			}
		}
	}
	for (MapObject* elem : mMapObjectList)
	{
		if (mCurrentLayer == Layer::Object) {
			elem->Render(hdc);
		}
		else {
			elem->AlphaRender(hdc);
		}
	}
	if (mCurrentTile)
	{
		mCurrentTile->SelectRender(hdc);
	}
	if (mCurrentPallete)
	{
		mCurrentPallete->Render(hdc, _mousePosition.x - 25, _mousePosition.y - 12);
		if (mCurrentPallete->GetType() == TileType::Normal) {
			wstring text = L"Normal";
			TextOut(hdc, 50, 100, text.c_str(), text.length());
		}
		else if (mCurrentPallete->GetType() == TileType::Slow) {
			wstring text = L"Slow";
			TextOut(hdc, 50, 100, text.c_str(), text.length());
		}
		else if (mCurrentPallete->GetType() == TileType::Block) {
			wstring text = L"Block";
			TextOut(hdc, 50, 100, text.c_str(), text.length());
		}
	}
	if (mCurrentMapObjectPallete)
	{
		mCurrentMapObjectPallete->Render(hdc, _mousePosition.x - 25, _mousePosition.y - 12);
	}
	if (mCurrentLayer == Layer::Object) {
		wstring text = L"Object Layer";
		TextOut(hdc, 50, 150, text.c_str(), text.length());
	}
	else if (mCurrentLayer==Layer::Tile) {
		wstring text = L"Tile Layer";
		TextOut(hdc, 50, 150, text.c_str(), text.length());
	}
	
	if (!mTabKey) {
		mSave->Render(hdc);
		mLoad->Render(hdc);
		mUndo->Render(hdc);
		mRedo->Render(hdc);
		mNext->Render(hdc);
		mChangeLayer->Render(hdc);

		if (mCurrentLayer == Layer::Tile)
		{
			for (int y = 0; y < mPalleteList.size(); y++)
			{
				for (int x = 0; x < mPalleteList[y].size(); x++)
				{
					mPalleteList[y][x]->Render(hdc);
				}
			}
		}
		else if (mCurrentLayer == Layer::Object)
		{
			for (int y = 0; y < mMapObjectPallete.size(); y++)
			{
				for (int x = 0; x < mMapObjectPallete[y].size(); x++)
				{
					mMapObjectPallete[y][x]->Render(hdc);
				}
			}
		}
	}
}
