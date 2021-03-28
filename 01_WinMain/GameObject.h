#pragma once
class GameObject
{
protected:
	string mName;
	float mX;
	float mY;
	float mSizeX;
	float mSizeY;
	RECT mRect;
	int mIndexX;
	int mIndexY;
	int mDamage=0;
	bool mIsTrashed;


	bool mIsActive;		//활성화 상태
	bool mIsDestroy;	//삭제할지 말지
public:
	GameObject();
	GameObject(const string& name);

	//순수가상함수 : 반드시 자식에서 해당 함수를 오버라이딩 해야함
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render(HDC hdc) = 0;
	virtual void Damage(int a) {}
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

	inline int GetIndexX()const { return mIndexX; }
	inline void SetIndexX(int x) { mIndexX = x; }
	inline int GetIndexY()const { return mIndexY; }
	inline void SetIndexY(int y) { mIndexY = y; }

	inline bool GetIsActive()const { return mIsActive; }
	inline void SetIsActive(bool b) { mIsActive = b; }
	inline bool GetIsDestroy()const { return mIsDestroy; }
	inline void SetIsDestroy(bool b) { mIsDestroy = b; }

	inline int GetDamage()const { return mDamage; }

	void SetObjectOnTile(int indexX, int indexY);
	virtual bool const GetIsTrashed() {return mIsTrashed;}
};

