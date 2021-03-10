#include "pch.h"
#include "CameraManager.h"

#include "Camera.h"
void CameraManager::Update()
{
	if (mMainCamera)
		mMainCamera->Update();
}
