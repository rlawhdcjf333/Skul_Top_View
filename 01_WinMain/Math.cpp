#include "pch.h"
#include "Math.h"

/*****************************************************************
## GetDistance ##
@@ aX : ������
@@ aY : ������
@@ bX : ��ǥ��
@@ bY : ��ǥ��

�� �� ������ ���̸� ���� ��ȯ�ϴ� �Լ�
******************************************************************/
float Math::GetDistance(float aX, float aY, float bX, float bY)
{
	float width = aX - bX;
	float height = aY - bY;

	float distance = sqrtf(width * width + height * height);

	return distance;
}

/*****************************************************************
## GetDistance ##
@@ aX : ������
@@ aY : ������
@@ bX : ��ǥ��
@@ bY : ��ǥ��

���������� ��ǥ������ ���ϴ� ������ ���ϴ� �Լ�
******************************************************************/
float Math::GetAngle(float aX, float aY, float bX, float bY)
{
	float width = bX - aX;
	float height = bY - aY;

	float distance = sqrtf(width * width + height * height);

	float angle = acos(width / distance);

	//��ǥ���� �� �Ʒ��� �ִٸ� ������ �ֱ�
	if (bY > aY)
	{
		angle = PI2 - angle;
	}

	return angle;
}

/*****************************************************************
## Lerp ##
@@ start : ���� ��
@@ end : �� ��
@@ ratio : ����(0~1)

���۰��� ������ ������ ����� ���� ��ȯ
******************************************************************/
float Math::Lerp(float start, float end, float ratio)
{
	return start + (end - start) * ratio;
}
