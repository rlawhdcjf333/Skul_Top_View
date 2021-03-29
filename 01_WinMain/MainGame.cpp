#include "pch.h"
#include "MainGame.h"
#include "Inventory.h"
#include "Image.h"
#include "GameScene.h"
#include "GameScene2.h"
#include "GameScene3.h"
#include "GameScene4.h"
#include "GameScene5.h"
#include "GameScene6.h"
#include "GameScene7.h"
#include "GameScene8.h"

/*
Initialize : ì´ˆê¸°??
?žìœ¼ë¡?ê²Œìž„???œìž‘?˜ê¸° ?„ì— ì´ˆê¸°??ë°??ì„±?€ ?¬ê¸°??ì§„í–‰
ê²Œìž„ ?œìž‘?˜ê¸° ?„ì— ???œë²ˆë§??¤í–‰?˜ëŠ” ?¨ìˆ˜
*/
void MainGame::Init()
{
	ShowCursor(false);
	mBackBuffer = new Image();
	mBackBuffer->CreateEmpty(WINSIZEX, WINSIZEY);
	IMAGEMANAGER->LoadFromFile(L"Cursor", Resources(L"Cursor.bmp"), 21, 21, true);
	IMAGEMANAGER->LoadFromFile(L"Tiles", Resources(L"Tiles.bmp"), 180, 180, 3, 4, true);
	IMAGEMANAGER->LoadFromFile(L"Tree2", ResourcesObject(L"Tree2.bmp"), 86, 142, true);
	IMAGEMANAGER->LoadFromFile(L"Tree3", ResourcesObject(L"Tree3.bmp"), 92, 190, true);
	IMAGEMANAGER->LoadFromFile(L"Tree9", ResourcesObject(L"Tree9.bmp"), 344, 290, true);
	IMAGEMANAGER->LoadFromFile(L"Bush6", ResourcesObject(L"Bush6.bmp"), 57, 48, true);
	IMAGEMANAGER->LoadFromFile(L"witch", ResourcesObject(L"witch.bmp"), 90, 90, true);
	IMAGEMANAGER->LoadFromFile(L"Branch1", ResourcesObject(L"Branch1.bmp"), 38, 23, true);
	IMAGEMANAGER->LoadFromFile(L"Branch2", ResourcesObject(L"Branch2.bmp"), 41, 22, true);
	IMAGEMANAGER->LoadFromFile(L"Branch3", ResourcesObject(L"Branch3.bmp"), 106, 67, true);
	IMAGEMANAGER->LoadFromFile(L"Branch4", ResourcesObject(L"Branch4.bmp"), 106, 67, true);
	IMAGEMANAGER->LoadFromFile(L"Door1_1", ResourcesObject(L"Door1_1.bmp"), 80, 279, true);
	IMAGEMANAGER->LoadFromFile(L"Door1_2", ResourcesObject(L"Door1_2.bmp"), 80, 279, true);
	IMAGEMANAGER->LoadFromFile(L"Door2_1", ResourcesObject(L"Door2_1.bmp"), 80, 279, true);
	IMAGEMANAGER->LoadFromFile(L"Door2_2", ResourcesObject(L"Door2_2.bmp"), 80, 279, true);
	IMAGEMANAGER->LoadFromFile(L"witch_tower", ResourcesObject(L"witch_tower.bmp"), 350, 333, true);
	IMAGEMANAGER->LoadFromFile(L"Building1", ResourcesObject(L"Building1.bmp"), 350, 383, true);
	IMAGEMANAGER->LoadFromFile(L"Building2", ResourcesObject(L"Building2.bmp"), 350, 393, true);
	IMAGEMANAGER->LoadFromFile(L"Building3", ResourcesObject(L"Building3.bmp"), 350, 275, true);
	IMAGEMANAGER->LoadFromFile(L"Building4", ResourcesObject(L"Building4.bmp"), 271, 448, true);
	IMAGEMANAGER->LoadFromFile(L"Flower1", ResourcesObject(L"Flower1.bmp"), 61, 26, true);
	IMAGEMANAGER->LoadFromFile(L"Flower2", ResourcesObject(L"Flower2.bmp"), 51, 32, true);
	IMAGEMANAGER->LoadFromFile(L"Flower3", ResourcesObject(L"Flower3.bmp"), 56, 36, true);
	IMAGEMANAGER->LoadFromFile(L"Flower4", ResourcesObject(L"Flower4.bmp"), 47, 30, true);
	IMAGEMANAGER->LoadFromFile(L"Tree4", ResourcesObject(L"Tree4.bmp"), 122, 278, true);
	IMAGEMANAGER->LoadFromFile(L"Tree5", ResourcesObject(L"Tree5.bmp"), 114, 266, true);
	IMAGEMANAGER->LoadFromFile(L"Tree6", ResourcesObject(L"Tree6.bmp"), 96, 212, true);
	IMAGEMANAGER->LoadFromFile(L"Tree7", ResourcesObject(L"Tree7.bmp"), 74, 206, true);
	IMAGEMANAGER->LoadFromFile(L"Tree8", ResourcesObject(L"Tree8.bmp"), 284, 226, true);
	IMAGEMANAGER->LoadFromFile(L"Tree10", ResourcesObject(L"Tree10.bmp"), 180, 217, true);
	IMAGEMANAGER->LoadFromFile(L"Tree11", ResourcesObject(L"Tree11.bmp"), 121, 103, true);
	IMAGEMANAGER->LoadFromFile(L"Tree12", ResourcesObject(L"Tree12.bmp"), 120, 100, true);
	IMAGEMANAGER->LoadFromFile(L"Post", ResourcesObject(L"Post.bmp"), 80, 100, true);
	IMAGEMANAGER->LoadFromFile(L"Bush1", ResourcesObject(L"Bush1.bmp"), 33, 27, true);
	IMAGEMANAGER->LoadFromFile(L"Bush2", ResourcesObject(L"Bush2.bmp"), 56, 46, true);
	IMAGEMANAGER->LoadFromFile(L"Bush3", ResourcesObject(L"Bush3.bmp"), 42, 22, true);
	IMAGEMANAGER->LoadFromFile(L"Bush4", ResourcesObject(L"Bush4.bmp"), 33, 24, true);
	IMAGEMANAGER->LoadFromFile(L"Bush5", ResourcesObject(L"Bush5.bmp"), 57, 40, true);
	IMAGEMANAGER->LoadFromFile(L"Bush7", ResourcesObject(L"Bush7.bmp"), 60, 38, true);

	IMAGEMANAGER->LoadFromFile(L"Disease", Resources(L"/skul/diseaseExplosion.bmp"), 2000, 200, 10, 1, true);
	IMAGEMANAGER->LoadFromFile(L"Fire", Resources(L"/skul/fireExplosion.bmp"), 2600, 200, 13, 1, true);
	mCursorImage = ImageManager::GetInstance()->FindImage(L"Cursor");
	mCamera = new Camera;
	mCamera->Init();
	SKUL->Init();
	CameraManager::GetInstance()->SetMainCamera(mCamera);

	//SceneManager::GetInstance()->AddScene(L"MapToolScene", new MapToolScene);
	SceneManager::GetInstance()->AddScene(L"GameScene", new GameScene);
	SceneManager::GetInstance()->AddScene(L"GameScene2", new GameScene2);
	SceneManager::GetInstance()->AddScene(L"GameScene3", new GameScene3);
	SceneManager::GetInstance()->AddScene(L"GameScene4", new GameScene4);
	SceneManager::GetInstance()->AddScene(L"GameScene5", new GameScene5);
	SceneManager::GetInstance()->AddScene(L"GameScene6", new GameScene6);
	SceneManager::GetInstance()->AddScene(L"GameScene7", new GameScene7);
	SceneManager::GetInstance()->AddScene(L"GameScene8", new GameScene8);
	SceneManager::GetInstance()->LoadScene(L"GameScene");

}

/*
Release : ë©”ëª¨ë¦??´ì œ????ë¶ˆëŸ¬ì£¼ëŠ” ?¨ìˆ˜
? ë‹ˆ?°ë¼ë©?OnDestroy
?´ë‹¹ ?´ëž˜???¸ìŠ¤?´ìŠ¤ê°€ ë©”ëª¨ë¦??´ì œ ???????œë²ˆ ?¸ì¶œ?´ì£¼???€??
*/
void MainGame::Release()
{
	Random::ReleaseInstance();	//?±ê????¸ìŠ¤?´ìŠ¤ ?? œ
	SKUL->Release();
	SafeDelete(mBackBuffer);
	SafeDelete(mCamera);
}

/*
Update : ë§??„ë ˆ???¤í–‰?˜ëŠ” ?¨ìˆ˜, ?¬ê¸°???°ì‚° ì²˜ë¦¬ ?œë‹¤.
*/
void MainGame::Update()
{
	INPUT->Update();
	SkulManager::GetInstance()->Update();
	if (SKUL->GetInventory()->GetIsToggle()==false)
	{
		CAMERA->Update();
		SceneManager::GetInstance()->Update();
	}
	//if (INPUT->GetKeyDown(VK_ESCAPE)) SceneManager::LoadScene();
}

/*
Render : ë§??„ë ˆ???¤í–‰?˜ëŠ” ?¨ìˆ˜, Updateê°€ ?ë‚˜ê³?Renderê°€ ?¤í–‰?œë‹¤.
?”ë©´??ê·¸ë ¤ì£¼ëŠ” ê²ƒë“¤?€ ?„ë? ?¬ê¸°??ì²˜ë¦¬

ë§¤ê°œë³€??hdc : ?ˆë„??ì°½ì˜ HDCê°€ ?¤ì–´??
*/
void MainGame::Render(HDC hdc)
{
	//ë°±ë²„?¼ì˜ HDC ê°€?¸ì˜¨??
	HDC backDC = mBackBuffer->GetHDC();
	//HDC ?ì—­???¹ì • ?‰ìœ¼ë¡?ë°€?´ë²„ë¦¬ëŠ” ?€??
	PatBlt(backDC, 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	// ==================================================
	{
		SceneManager::GetInstance()->Render(backDC);
		SKUL->Render(backDC);
		RenderTime(backDC);
		mCursorImage->Render(backDC, nonC_mousePosition.x, nonC_mousePosition.y);
	}
	//====================================================
	//?„ë©´ë²„í¼ ?´ìš©???ˆë„??ì°½ì— ê³ ì† ë³µì‚¬
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

