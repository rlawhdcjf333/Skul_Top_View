#pragma once

/*
## ObjectManager ##
GameObject �����ϴ� Ŭ����
*/

//enum (������) : ������ �ٸ� �̸����� ǥ���ϴ� ��
enum class ObjectLayer : int
{
	Background = 0, 
	Player = 1,
	Enemy = 2,
	UI = 3,
	Player_Bullet = 4,
	Enemy_Bullet,
	MapObject,
	End
};

class ObjectManager
{
	Singleton(ObjectManager)

private:
	typedef map<ObjectLayer, vector<class GameObject*>>::iterator ObjectIter;
	map<ObjectLayer, vector<class GameObject*>> mObjectList;
	vector<GameObject*> mRenderList;
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

};

#define Obj ObjectManager::GetInstance()