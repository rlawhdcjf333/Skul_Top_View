#include "pch.h"
#include "ObjectManager.h"
#include "Bullet.h"
#include "GameObject.h"
ObjectManager::ObjectManager()
{
	//ObjectLayer ���� ���� �ϳ��� �ʿ� ���� �ִ´�.
	for (int i = 0; i < (int)ObjectLayer::End; ++i)
	{
		vector<GameObject*> emptyVector;
		mObjectList.insert(make_pair((ObjectLayer)i, emptyVector));
	}
}
void ObjectManager::Init()
{
	ObjectIter iter = mObjectList.begin();
	for (; iter != mObjectList.end(); ++iter)
	{
		for (int i = 0; i < iter->second.size(); ++i)
		{	
			iter->second[i]->Init();
		}
	}
}

void ObjectManager::Release()
{
	ObjectIter iter = mObjectList.begin();
	for (; iter != mObjectList.end(); ++iter)
	{
		for (int i = 0; i < iter->second.size(); ++i)
		{
			iter->second[i]->Release();
			SafeDelete(iter->second[i]);
		}
	}
	mObjectList.clear();
}

void ObjectManager::Update()
{
	ObjectIter iter = mObjectList.begin();
	for (; iter != mObjectList.end(); ++iter)
	{
		for (int i = 0; i < iter->second.size(); ++i)
		{
			if (iter->second[i]->GetIsDestroy() == true)
			{
				iter->second[i]->Release();
				SafeDelete(iter->second[i]);
				iter->second.erase(iter->second.begin() + i);
				--i;
				continue;
			}
			if (iter->second[i]->GetIsActive() == true)
			{
				iter->second[i]->Update();
			}
		}
	}
	IntersectObject();

	mRenderList.clear();
	for (int i = 0; i < (int)ObjectLayer::End; i++)
	{
		if (i == 0) continue;

		for (GameObject* elem : mObjectList[(ObjectLayer)i])
		{
			mRenderList.push_back(elem);
		}

	}
	function <bool(GameObject* a, GameObject* b)> comp = [](GameObject* a, GameObject* b)
	{
		int aY = a->GetRect().bottom;
		int bY = b->GetRect().bottom;

		return aY<bY;
	};
	sort(mRenderList.begin(), mRenderList.end(), comp);

}

void ObjectManager::Render(HDC hdc)
{
	ObjectIter iter = mObjectList.begin();
	for (; iter != mObjectList.end(); ++iter)
	{
		for (int i = 0; i < iter->second.size(); ++i)
		{
			if (iter->second[i]->GetIsActive() == true)
			{	
				iter->second[i]->Render(hdc);
			}
		}
	}

	for (GameObject* elem : mObjectList[ObjectLayer::Background])
	{
		if (elem->GetIsActive() == true)
		{
			elem->Render(hdc);
		}
	}
	for (GameObject* elem : mRenderList)
	{
		if (elem->GetIsActive() == true)
		{
			elem->Render(hdc);
		}
	}

}

void ObjectManager::IntersectObject()
{
	RECT tmp;

	for (GameObject* elem : mObjectList[ObjectLayer::Player_Bullet])
	{
		for (GameObject* elemelem : mObjectList[ObjectLayer::Enemy])
		{

			RECT playerBullet = elem->GetRect();
			RECT enemy = elemelem->GetRect();
			if (IntersectRect(&tmp, &playerBullet, &enemy))
			{
				elemelem->Damage(elem->GetDamage());
				elem->Damage(0);
			}
		}
	}
}

void ObjectManager::AddObject(ObjectLayer layer, GameObject * object)
{
	//map�� �迭�����ڰ� ���ǵǾ� �ִ�. 
	//��, ���� �迭ó�� �����ϴ°� �ƴ϶�.[]������ ���ο� find�Լ��� �Ἥ ������
	//�׷��� �ᱹ find���°Ŷ� ����ѵ�, �ٸ����̶�� �Ѵٸ� �ش� Ű���� �����Ͱ� 
	//������ ���� �����ع���. �����ؾ���
	mObjectList[layer].push_back(object);
}

//�ش� �̸��� ������Ʈ ã�ƿ���
GameObject * ObjectManager::FindObject(const string & name)
{
	ObjectIter iter = mObjectList.begin();
	for (; iter != mObjectList.end(); ++iter)
	{
		for (int i = 0; i < iter->second.size(); ++i)
		{
			if (iter->second[i]->GetName() == name)
			{
				return iter->second[i];
			}
		}
	}
	return nullptr;
}

//�ش� �̸��� ������Ʈ ã��
GameObject * ObjectManager::FindObject(ObjectLayer layer, const string & name)
{
	ObjectIter iter = mObjectList.find(layer);
	for (int i = 0; i < iter->second.size(); ++i)
	{
		if (iter->second[i]->GetName() == name)
		{
			return iter->second[i];
		}
	}
	return nullptr;
}

vector<class GameObject*> ObjectManager::FindObjects(const string & name)
{
	vector<GameObject*> result;

	ObjectIter iter = mObjectList.begin();
	for (; iter != mObjectList.end(); ++iter)
	{
		for (int i = 0; i < iter->second.size(); ++i)
		{
			if (iter->second[i]->GetName() == name)
			{
				result.push_back(iter->second[i]);
			}
		}
	}

	return result;
}

vector<class GameObject*> ObjectManager::FindObjects(ObjectLayer layer, const string & name)
{
	vector<GameObject*> result;
	ObjectIter iter = mObjectList.find(layer);
	for (int i = 0; i < iter->second.size(); ++i)
	{
		if (iter->second[i]->GetName() == name)
		{
			result.push_back(iter->second[i]); 
		}
	}
	return result;
}

vector<class GameObject*> ObjectManager::GetObjectList(ObjectLayer layer)
{
	return mObjectList[layer];
}