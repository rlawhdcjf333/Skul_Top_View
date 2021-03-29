#include "pch.h"
#include "MainScene.h"
#

void MainScene::Init()
{
	IMAGEMANAGER->LoadFromFile(L"Main", Resources(L"Main.bmp"), 1280, 750, false);
	mBack = IMAGEMANAGER->FindImage(L"Main");
	IMAGEMANAGER->LoadFromFile(L"Anybutton", Resources(L"Anybutton2.bmp"), 230, 48, true);
	mAnyButton = IMAGEMANAGER->FindImage(L"Anybutton");
	CAMERA->ChangeMode(Camera::Mode::Free);
}

void MainScene::Update()
{
	if (INPUT->GetKey(WM_KEYDOWN))
	{
		SceneManager::GetInstance()->LoadScene(L"GameScene");
	}
}

void MainScene::Render(HDC hdc)
{
	mBack->Render(hdc, 0, 0);
	mAnyButton->Render(hdc, 525, 505);
}

void MainScene::Release()
{

}
