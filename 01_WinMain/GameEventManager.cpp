#include "pch.h"
#include "GameEventManager.h"

#include "GameEvent.h"

GameEventManager::GameEventManager()
{
}

GameEventManager::~GameEventManager()
{
	RemoveAllEvent();
}

void GameEventManager::RemoveAllEvent()
{
	for (int i = 0; i < mEventQueue.size(); ++i)
		SafeDelete(mEventQueue[i]);
	mEventQueue.clear();
}

void GameEventManager::Update()
{
	if (mEventQueue.size() == 0)
		return;

	bool isEnd = mEventQueue[0]->Update();
	if (isEnd == true)
	{
		SafeDelete(mEventQueue[0]);
		mEventQueue.erase(mEventQueue.begin());

		if (mEventQueue.size() != 0)
		{
			mEventQueue[0]->Start();
		}
	}
}

void GameEventManager::PushEvent(IEvent * event)
{
	if (mEventQueue.size() == 0)
	{
		event->Start();
	}
	mEventQueue.push_back(event);
}
