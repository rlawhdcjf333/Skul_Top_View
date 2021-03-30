#include "pch.h"
#include "MainScene.h"
#include "Animation.h"

void MainScene::Init()
{
	IMAGEMANAGER->LoadFromFile(L"Main", Resources(L"Main.bmp"), 1280, 750, false);
	mBack = IMAGEMANAGER->FindImage(L"Main");
	IMAGEMANAGER->LoadFromFile(L"Anybutton", Resources(L"Anybutton.bmp"), 460, 48, 2, 1, true);
	mAnyButton = IMAGEMANAGER->FindImage(L"Anybutton");
	CAMERA->ChangeMode(Camera::Mode::Free);
	mCount = 3.f;
	mAnm = new Animation();
	mAnm->InitFrameByStartEnd(0, 0, 1, 0, false);
	mAnm->SetIsLoop(true);
	mAnm->SetFrameUpdateTime(0.4f);
	mAnm->Play();
}

void MainScene::Update()
{
	mAnm->Update();
	if(mCount>0)
	mCount += -dTime;
	if (mCount <= 0)
	{
		if (INPUT->GetAllKeyDown())
		{
			SceneManager::GetInstance()->LoadScene(L"GameScene");
		}
	}
}

void MainScene::Render(HDC hdc)
{
	mBack->Render(hdc, 0, 0);
	if(mCount<=2)
	mAnyButton->FrameRender(hdc, 525, 505,mAnm->GetNowFrameX(),0);
}

void MainScene::Release()
{
}
