#pragma once
//�����!!!
class GameObject
{
protected:
	string mName;
	float mX;
	float mY;
	float mSizeX;
	float mSizeY;
	RECT mRect;

	bool mIsActive;		//Ȱ��ȭ ����
	bool mIsDestroy;	//�������� ����
public:
	GameObject();
	GameObject(const string& name);

	//���������Լ� : �ݵ�� �ڽĿ��� �ش� �Լ��� �������̵� �ؾ���
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render(HDC hdc) = 0;
public:
	inline string GetName()const { return mName; }
	inline void SetName(const string& name) { mName = name; }
	inline float GetX()const { return mX; }
	inline void SetX(float x) { mX = x; }
	inline float GetY()const { return mY; }
	inline void SetY(float y) { mY = y; }
	inline float GetSizeX()const { return mSizeX; }
	inline void SetSizeX(float x) { mSizeX = x; }
	inline float GetSizeY()const { return mSizeY; }
	inline void SetSizeY(float y) { mSizeY = y; }
	inline RECT GetRect()const { return mRect; }

	inline bool GetIsActive()const { return mIsActive; }
	inline void SetIsActive(bool b) { mIsActive = b; }
	inline bool GetIsDestroy()const { return mIsDestroy; }
	inline void SetIsDestroy(bool b) { mIsDestroy = b; }
};

