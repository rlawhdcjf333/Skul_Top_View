#pragma once
class GameEventManager
{
	Singleton(GameEventManager)
private:
	//�� queue��� �ڷᱸ�� �����ؼ� queue�� �ٲ����
	vector<class IEvent*> mEventQueue;
public:
	GameEventManager();
	~GameEventManager();

	void RemoveAllEvent();
	void Update();

	void PushEvent(class IEvent* event);
};

