#include "pch.h"
#include "Random.h"
#include <ctime>
//static ������ �ݵ�� cpp���� �ʱ�ȭ �� ������ ���־�� �Ѵ�.
Random* Random::_instance = NULL;

Random::Random()
{
	srand(time(NULL));
}

