#pragma once
class ThreadManager
{
	Singleton(ThreadManager)
private:
	bool mIsThreadPlay = true;
	queue<function<void(void)>> mFuncs;
	thread* mLoadingThread;
public :
	~ThreadManager();
	void LoadStartFunts();
	void SetFunts(const function<void(void)>& funts);
	void Loding();
	void LoadStartEnd();
	void Update();

	int GetIsListSize() { return mFuncs.size(); }
	bool GetIsThreadPlay() { return mIsThreadPlay; }

};