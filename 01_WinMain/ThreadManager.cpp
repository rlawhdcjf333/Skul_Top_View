#include "pch.h"
#include "ThreadManager.h"

ThreadManager::~ThreadManager() {
	mLoadingThread->join();
	delete mLoadingThread;
	mLoadingThread = nullptr;
	mIsThreadPlay = false;
}
void ThreadManager::SetFunts(const function<void(void)>& func)
{
	mFuncs.push(func);
}

void ThreadManager::LoadStartFunts()
{
	if (mLoadingThread == nullptr) {
		function<void(void)> threadFunc = bind(&ThreadManager::Loding, this);
		//스레드는 메모리 할당해주는 순간부터 바로 돌기 시작한다.
		mLoadingThread = new thread(threadFunc);
	}
}
void ThreadManager::Loding() {
	while (mFuncs.size() != 0) {
		function<void(void)> startFunc = mFuncs.front();
		startFunc();
		mFuncs.pop();
	}
}
void ThreadManager::LoadStartEnd()
{
	if (mLoadingThread != nullptr) {
		mLoadingThread->join();
		delete mLoadingThread;
		mLoadingThread = nullptr;
		mIsThreadPlay = false;
	}
}
void ThreadManager::Update() {
	if (mFuncs.size() == 0) {
		mIsThreadPlay = false;
		//LoadStartEnd();
	}
	else if (mFuncs.size() > 0) {
		mIsThreadPlay = true;
		LoadStartFunts();
	}
}