#include "pch.h"
#include "MainScene.h"

void MainScene::Init()
{
	IMAGEMANAGER->LoadFromFile(L"Main", Resources(L"Main.bmp"), 1280, 740, false);
	mBack = IMAGEMANAGER->FindImage(L"Main");
}

void MainScene::Update()
{
	if (INPUT->GetKeyDown(WM_KEYDOWN))
	{
		SceneManager::GetInstance()->LoadScene(L"GameScene");
	}
}

void MainScene::Render(HDC hdc)
{
	mBack->Render(hdc, 0, 0);
}

void MainScene::Release()
{

}
