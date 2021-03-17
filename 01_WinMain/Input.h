#pragma once
#include <bitset>

class Input final
{
public:
	static Input* GetInstance()
	{
		static Input _instance;
		return &_instance;
	}
private:
	enum : int {KeyMax = 256};
private:
	//중복 키 허용을 위해 이전 키와 현재 키 변수 사용
	std::bitset<KeyMax> mKeyPast;
	std::bitset<KeyMax> mKeyCurrent;
private:

	Input();
	virtual ~Input();
public:
	void Update();
	//한번 눌렸다면
	bool GetKeyDown(const int& key);
	//키에서 손을 땠다면
	bool GetKeyUp(const int& key);
	//키를 누르고 있다면
	bool GetKey(const int& key);
	//토글 키
	bool ToggleKey(const int& key);


};

#define INPUT Input::GetInstance()
