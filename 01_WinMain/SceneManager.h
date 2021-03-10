#pragma once
#include <thread>
class Scene;
class SceneManager
{
	Singleton(SceneManager)
private:
	typedef map<wstring, Scene*>::iterator SceneIter;
	map<wstring, Scene*> mSceneList;
	Scene* mCurrentScene;						//���� ��

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
	//�ε����� �ε��ϴ� �Լ�
	void LoadScene(const wstring& sceneName);
	void LoadScene(const wstring& targetSceneName, const wstring& loadingSceneName);
private:
	//�����忡 ���� ���� �Լ� : �� ������ �����尡 �۾� ó�� �� �Լ�
	void LoadingThread();
};

