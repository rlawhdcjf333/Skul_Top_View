#include "pch.h"
#include "ObjectManager.h"
#include "Bullet.h"
#include "GameObject.h"
#include "Enemy.h"
#include "Effect.h"

ObjectManager::ObjectManager()
{
	//ObjectLayer 별로 벡터 하나씩 맵에 집어 넣는다.
	for (int i = 0; i < (int)ObjectLayer::End; ++i)
	{
		vector<GameObject*> emptyVector;
		mObjectList.insert(make_pair((ObjectLayer)i, emptyVector));
	}
}
void ObjectManager::Init()
{
	mTimeStop = false;

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
			if (iter->first == ObjectLayer::Player) continue;
			if (iter->first == ObjectLayer::Item && iter->second[i]->GetIsTrashed() == false) continue;
			iter->second[i]->Release();
			SafeDelete(iter->second[i]);
		}
	}
	mObjectList.clear();
	mRenderList.clear();
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
				if (mTimeStop == false)
				{
					iter->second[i]->Update();
				}
				else
				{
					if (iter->first == ObjectLayer::Player)
					{
						iter->second[i]->Update();
					}
				}
			}
		}
	}

	IntersectObject();

	RECT cameraRect = CAMERA->GetRect();
	mRenderList.clear();
	for (int i = 0; i < (int)ObjectLayer::End; i++)
	{
		if (i == (int)ObjectLayer::Background) continue;
		if (i == (int)ObjectLayer::Effect) continue;

		for (GameObject* elem : mObjectList[(ObjectLayer)i])
		{
			if (elem->GetRect().left<cameraRect.right && elem->GetRect().right > cameraRect.left)
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
	for (GameObject* elem : mObjectList[ObjectLayer::Background]) //반드시 뒤에 와야되는 오브젝트의 렌더링
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
	for (GameObject* elem : mObjectList[ObjectLayer::Effect]) //반드시 앞에 와야되는 오브젝트의 렌더링
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
		Bullet* downcast = (Bullet*)elem;
		if (downcast->GetType() == BulletType::Protect) continue;
		if (downcast->GetType() == BulletType::Barricade) continue;

		for (GameObject* elemelem : mObjectList[ObjectLayer::Enemy])
		{
			
			RECT playerBullet = elem->GetRect();
			RECT enemy = elemelem->GetRect();
			if (IntersectRect(&tmp, &playerBullet, &enemy))
			{
				new Effect(L"SkulHitEffect", elemelem->GetRect().left, elemelem->GetRect().top, EffectType::Normal);
				int dam;
				Enemy* tmp;
				switch (downcast->GetType())
				{
				case BulletType::Mark:
					dam = elem->GetDamage();
					tmp = dynamic_cast<Enemy*>(elemelem);
					tmp->Mark(dam, [=]() {
						tmp->Explosion(dam, 1); new Effect(L"ClownMark", tmp->GetRect().left, tmp->GetRect().top, EffectType::Normal);});
					elem->Damage(0);
					break;

				default:
					elemelem->Damage(elem->GetDamage());
					elem->Damage(0);

					break;

				}
			}
		}
	}
}

void ObjectManager::AddObject(ObjectLayer layer, GameObject * object)
{
	//map도 배열연산자가 정의되어 있다. 
	//단, 실제 배열처럼 동작하는게 아니라.[]연산자 내부에 find함수를 써서 동작함
	//그래서 결국 find쓰는거랑 비슷한데, 다른점이라고 한다면 해당 키값의 데이터가 
	//없으면 새로 생성해버림. 주의해야함
	mObjectList[layer].push_back(object);
}

//해당 이름의 오브젝트 찾아오기
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

//해당 이름의 오브젝트 찾기
GameObject * ObjectManager::FindObject(ObjectLayer layer, const string & name)
{
	ObjectIter iter = mObjectList.find(layer);
	if (iter == mObjectList.end()) {
		return nullptr;
	}

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

void ObjectManager::ReleaseObject(ObjectLayer layer, const string& name)
{
	ObjectIter iter = mObjectList.find(layer);
	for (int i = 0; i < iter->second.size(); ++i)
	{
		if (iter->second[i]->GetName() == name)
		{
			iter->second[i]->Release();
			SafeDelete(iter->second[i]);
			iter->second.erase(iter->second.begin() + i);
		}
	}
}

vector<class GameObject*>* ObjectManager::GetObjectListPt(ObjectLayer layer)
{
	return &mObjectList[layer];
}
