#pragma once
class GameEventManager
{
	Singleton(GameEventManager)
private:
	//얘 queue라는 자료구조 공부해서 queue로 바꿔오기
	vector<class IEvent*> mEventQueue;
public:
	GameEventManager();
	~GameEventManager();

	void RemoveAllEvent();
	void Update();

	void PushEvent(class IEvent* event);
};

