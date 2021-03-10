#include "pch.h"
#include "Random.h"
#include <ctime>
//static 변수는 반드시 cpp에서 초기화 및 선언을 해주어야 한다.
Random* Random::_instance = NULL;

Random::Random()
{
	srand(time(NULL));
}

