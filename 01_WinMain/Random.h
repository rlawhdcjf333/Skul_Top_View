#pragma once

/*
�̱��� ���� : �� �ϳ��� �ν��Ͻ����� ������ ����� ���������� �����ϱ� ���ϰ� 
�ϴ� ����
*/

class Random
{
	//���� �������� �տ��� s�Ǵ� _���δ�
	static Random* _instance;
public:
	//�̱��� �ν��Ͻ� ������ �Լ�
	static Random* GetInstance()
	{
		if (_instance == NULL)
		{
			_instance = new Random();
		}
		return _instance;
	}

	static void ReleaseInstance()
	{
		if (_instance != NULL)
		{
			delete _instance;
			_instance = NULL;
		}
	}
public:
	Random();

	//0~num������ ������ ��ȯ
	inline int RandomInt(int num) { return rand() % num; }	
	//startNum ~ endNum������ ������ ��ȯ�ϴ� �Լ�
	inline int RandomInt(int startNum, int endNum)
	{
		return rand() % (endNum - startNum + 1) + startNum;
	}
};

