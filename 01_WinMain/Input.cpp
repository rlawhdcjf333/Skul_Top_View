#include "pch.h"
#include "Input.h"

/****************************************************
## Input ##
*****************************************************/
Input::Input()
{
	//ó���� ��� Ű�� �������� ���� ���·� �ʱ�ȭ
	mKeyCurrent.reset();
	mKeyPast.reset();

}
Input::~Input() {}

/****************************************************
## Update ##
*****************************************************/
void Input::Update()
{
	//��� ����Ű�� ����Ű�� �������ش�
	mKeyPast = mKeyCurrent;
}


bool Input::GetKeyDown(const int& key)
{
	//�ش� Ű�� ���� ����
	if (GetAsyncKeyState(key) & 0x8000)
	{
		//�� Ű�� �������� �ʾҴٸ�
		if (!mKeyPast[key])
		{
			//����Ű�� ���� ���·� �ٲٰ� return true
			mKeyCurrent.set(key, true);
			return true;
		}
	}
	//�ش� Ű�� ������ ���� ����
	else mKeyCurrent.set(key, false);   //����Ű�� ������ ���� ���·� �ٲ�

	return false;
}

bool Input::GetKeyUp(const int& key)
{
	//�ش� Ű�� ���� ����					����Ű�� ���� ���·� �ٲ�
	if (GetAsyncKeyState(key) & 0x8000) mKeyCurrent.set(key, true);
	//�ش� Ű�� ������ ���� ����
	else
	{
		//�� Ű�� �����־��ٸ�
		if (mKeyPast[key])
		{
			//����Ű�� ������ ���� ���·� �ٲٰ� return true
			mKeyCurrent.set(key, false);
			return true;
		}
	}

	return false;
}

bool Input::GetKey(const int& key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;

	return false;
}

bool Input::ToggleKey(const int& key)
{
	if (GetKeyState(key) & 0x0001) return true;

	return false;
}