#include "pch.h"
#include "SceneManager.h"

#include "Scene.h"
#include "LoadingScene.h"

SceneManager::SceneManager()
	:mCurrentScene(nullptr),mLoadingThread(nullptr),mTargetScene(nullptr),mLoadingScene(nullptr),
mIsLoadingEnd(false){}

SceneManager::~SceneManager()
{
	SceneIter iter = mSceneList.begin();
	for (; iter != mSceneList.end(); ++iter)
	{
		//iter->second->Release();
		SafeDelete(iter->second);
	}
}

void SceneManager::Update()
{
	if (mCurrentScene != nullptr)
		mCurrentScene->Update();
}

void SceneManager::Render(HDC hdc)
{
	if (mCurrentScene != nullptr)
		mCurrentScene->Render(hdc);
}

void SceneManager::AddScene(const wstring& sceneName,Scene * scene)
{
	SceneIter iter = mSceneList.find(sceneName);
	//�̹� �ش��̸��� �����Ͱ� �ʾȿ� �ִ�
	if (iter != mSceneList.end())
		return;

	mSceneList.insert(make_pair(sceneName, scene));
}

void SceneManager::LoadScene(const wstring & sceneName)
{
	SceneIter iter = mSceneList.find(sceneName);
	//��ã������ return (���ư���)
	if (iter == mSceneList.end())
		return;

	if (mCurrentScene == iter->second)
		return;
	
	Scene* targetScene = iter->second;

	if (mCurrentScene)
		mCurrentScene->Release();

	targetScene->Init();

	mCurrentScene = targetScene;
}

//�� �����ؼ� ���. �� �𸣰� ���� ���� ������ �и�
void SceneManager::LoadScene(const wstring & targetSceneName, const wstring & loadingSceneName)
{
	SceneIter iter = mSceneList.find(targetSceneName);
	if (iter != mSceneList.end())
		mTargetScene = iter->second;

	iter = mSceneList.find(loadingSceneName);
	if (iter != mSceneList.end())
		mLoadingScene = iter->second;

	if (mTargetScene == nullptr || mLoadingScene == nullptr ||
		mTargetScene == mCurrentScene || mLoadingScene == mCurrentScene)
	{
		mTargetScene = nullptr;
		mLoadingScene = nullptr;
		return;
	}

	function<void(void)> threadFunc = bind(&SceneManager::LoadingThread, this);
	//������� �޸� �Ҵ����ִ� �������� �ٷ� ���� �����Ѵ�.
	mLoadingThread = new thread(threadFunc);
}

void SceneManager::LoadingThread()
{
	//����� ����!! ã�ƺ�!

	Scene* prevScene = mCurrentScene;
	
	mCurrentScene = mLoadingScene;
	mCurrentScene->Init();

	prevScene->Release();

	mTargetScene->Init();
	
	mCurrentScene = mTargetScene;


	mLoadingScene->Release();

	mTargetScene = nullptr;
	mLoadingScene = nullptr;
	mIsLoadingEnd = true;
}
