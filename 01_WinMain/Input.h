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
	//�ߺ� Ű ����� ���� ���� Ű�� ���� Ű ���� ���
	std::bitset<KeyMax> mKeyPast;
	std::bitset<KeyMax> mKeyCurrent;
private:

	Input();
	virtual ~Input();
public:
	void Update();
	//�ѹ� ���ȴٸ�
	bool GetKeyDown(const int& key);
	//Ű���� ���� ���ٸ�
	bool GetKeyUp(const int& key);
	//Ű�� ������ �ִٸ�
	bool GetKey(const int& key);
	//��� Ű
	bool ToggleKey(const int& key);


};

#define _Input Input::Get()
