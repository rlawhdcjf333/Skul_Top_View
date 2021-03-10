#pragma once
#include <thread>
class Scene;
class SceneManager
{
	Singleton(SceneManager)
private:
	typedef map<wstring, Scene*>::iterator SceneIter;
	map<wstring, Scene*> mSceneList;
	Scene* mCurrentScene;						//현재 씬

	class Scene* mTargetScene;
	class Scene* mLoadingScene;
	thread* mLoadingThread;
	bool mIsLoadingEnd;
public:
	SceneManager();
	~SceneManager();

	void Update();
	void Render(HDC hdc);

	void AddScene(const wstring& sceneName,Scene* scene);
	//로딩없이 로드하는 함수
	void LoadScene(const wstring& sceneName);
	void LoadScene(const wstring& targetSceneName, const wstring& loadingSceneName);
private:
	//스레드에 집어 넣을 함수 : 즉 실제로 스레드가 작업 처리 할 함수
	void LoadingThread();
};

