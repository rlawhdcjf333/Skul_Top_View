#pragma once
class IntersectManager
{
	Singleton(IntersectManager)
private:
	GameObject* mPlayer;
	vector<GameObject*>* mEnemy;
	vector<GameObject*>* mPlayerBullets;
	vector<GameObject*>* mEnemyBullets;
	vector<GameObject*>* mObject;
public :
	IntersectManager();
	void IntersectBullet();
};

