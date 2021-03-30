#pragma once

/*
## ObjectManager ##
GameObject 관리하는 클래스
*/

//enum (열거자) : 정수를 다른 이름으로 표현하는 것
enum class ObjectLayer : int
{
	Background = 0, 
	Player,
	Condition,
	Enemy,
	UI,
	Player_Bullet,
	Enemy_Bullet,
	MapObject,
	Effect,
	Door,
	Item,
	Font,
	End
};

class ObjectManager
{
	Singleton(ObjectManager)

private:
	typedef map<ObjectLayer, vector<class GameObject*>>::iterator ObjectIter;
	map<ObjectLayer, vector<class GameObject*>> mObjectList;
	vector<GameObject*> mRenderList;

	bool mTimeStop;

public:
	ObjectManager();

	void Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	void IntersectObject();
	void AddObject(ObjectLayer layer, class GameObject* object);
	class GameObject* FindObject(const string& name);
	class GameObject* FindObject(ObjectLayer layer, const string& name);
	vector<class GameObject*> FindObjects(const string& name);
	vector<class GameObject*> FindObjects(ObjectLayer layer, const string& name);
	vector<class GameObject*> GetObjectList(ObjectLayer layer);
	void ReleaseObject(ObjectLayer layer, const string& name);
	void DeleteSkul(class Player* skul);

	vector<class GameObject*>* GetObjectListPt(ObjectLayer layer);

	void SetTimeStop(bool b) { mTimeStop = b; }

	bool EnemyHitCheck();
	void DamageUpToEnemy(int val);

};

#define Obj ObjectManager::GetInstance()