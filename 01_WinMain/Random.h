#pragma once

/*
싱글턴 패턴 : 단 하나의 인스턴스만을 가지게 만들고 전역적으로 접근하기 편하게 
하는 패턴
*/

class Random
{
	//나는 전역변수 앞에는 s또는 _붙인다
	static Random* _instance;
public:
	//싱글톤 인스턴스 접근자 함수
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

	//0~num까지의 랜덤값 반환
	inline int RandomInt(int num) { return rand() % num; }	
	//startNum ~ endNum까지의 랜덤값 반환하는 함수
	inline int RandomInt(int startNum, int endNum)
	{
		return rand() % (endNum - startNum + 1) + startNum;
	}
};

