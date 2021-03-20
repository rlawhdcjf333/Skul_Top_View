#include "pch.h"
#include "MainGame.h"

#include "Image.h"
#include "GameScene.h"

/*
Initialize : 초기화
앞으로 게임을 시작하기 전에 초기화 및 생성은 여기서 진행
게임 시작하기 전에 딱 한번만 실행되는 함수
*/
void MainGame::Init()
{
	ShowCursor(false);
	mBackBuffer = new Image();
	mBackBuffer->CreateEmpty(WINSIZEX, WINSIZEY);
	IMAGEMANAGER->LoadFromFile(L"Cursor", Resources(L"Cursor.bmp"), 21, 21, true);
	IMAGEMANAGER->LoadFromFile(L"Tiles", Resources(L"tile_test.bmp"), 372, 372, 3, 6, true);
	IMAGEMANAGER->LoadFromFile(L"ForestObject", Resources(L"forestObject.bmp"), 360, 300, 3, 5, true);
	IMAGEMANAGER->LoadFromFile(L"Tree1", ResourcesObject(L"Tree1.bmp"), 200, 167, true);
	IMAGEMANAGER->LoadFromFile(L"Tree2", ResourcesObject(L"Tree2.bmp"), 86, 142, true);
	IMAGEMANAGER->LoadFromFile(L"Tree3", ResourcesObject(L"Tree3.bmp"), 92, 190, true);
	IMAGEMANAGER->LoadFromFile(L"Tree9", ResourcesObject(L"Tree9.bmp"), 344, 290, true);
	IMAGEMANAGER->LoadFromFile(L"Bush6", ResourcesObject(L"Bush6.bmp"), 57, 48, true);
	mCursorImage = ImageManager::GetInstance()->FindImage(L"Cursor");
	mCamera = new Camera;
	mCamera->Init();
	CameraManager::GetInstance()->SetMainCamera(mCamera);

	//SceneManager::GetInstance()->AddScene(L"MapToolScene", new MapToolScene);
	SceneManager::GetInstance()->AddScene(L"GameScene", new GameScene);
	SceneManager::GetInstance()->LoadScene(L"GameScene");

}

/*
Release : 메모리 해제할 때 불러주는 함수
유니티라면 OnDestroy
해당 클래스 인스턴스가 메모리 해제 될 때 단 한번 호출해주는 녀석
*/
void MainGame::Release()
{
	Random::ReleaseInstance();	//싱글톤 인스턴스 삭제

	SafeDelete(mBackBuffer);
	SafeDelete(mCamera);
}

/*
Update : 매 프레임 실행되는 함수, 여기서 연산 처리 한다.
*/
void MainGame::Update()
{
	CAMERA->Update();
	INPUT->Update();
	SkulManager::GetInstance()->Update();
	SceneManager::GetInstance()->Update();
}

/*
Render : 매 프레임 실행되는 함수, Update가 끝나고 Render가 실행된다.
화면에 그려주는 것들은 전부 여기서 처리

매개변수 hdc : 윈도우 창의 HDC가 들어옴
*/
void MainGame::Render(HDC hdc)
{
	//백버퍼의 HDC 가져온다
	HDC backDC = mBackBuffer->GetHDC();
	//HDC 영역을 특정 색으로 밀어버리는 녀석
	PatBlt(backDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	// ==================================================
	{
		SceneManager::GetInstance()->Render(backDC);
		mCursorImage->Render(backDC, nonC_mousePosition.x, nonC_mousePosition.y);
		//RenderTime(backDC);
	}
	//====================================================
	//후면버퍼 내용을 윈도우 창에 고속 복사
	mBackBuffer->Render(hdc, 0, 0);
}

void MainGame::RenderTime(HDC hdc)
{
	float worldTime = Time::GetInstance()->GetWorldTime();
	float deltaTime = Time::GetInstance()->DeltaTime();
	ULONG fps = Time::GetInstance()->GetmFrameRate();
	wstring strWorldTime = L"WorldTime : " + to_wstring(worldTime);
	wstring strDeltaTime = L"DeltaTime : " + to_wstring(deltaTime);
	wstring strFPS = L"FPS : " + to_wstring(fps);

	TextOut(hdc, 10, 10, strWorldTime.c_str(), strWorldTime.length());
	TextOut(hdc, 10, 25, strDeltaTime.c_str(), strDeltaTime.length());
	TextOut(hdc, 10, 40, strFPS.c_str(), strFPS.length());
}

