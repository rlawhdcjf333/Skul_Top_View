#include "pch.h"
#include "Math.h"

/*****************************************************************
## GetDistance ##
@@ aX : 기준점
@@ aY : 기준점
@@ bX : 목표점
@@ bY : 목표점

두 점 사이의 길이를 구해 반환하는 함수
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
@@ aX : 기준점
@@ aY : 기준점
@@ bX : 목표점
@@ bY : 목표점

기준점에서 목표점으로 향하는 각도를 구하는 함수
******************************************************************/
float Math::GetAngle(float aX, float aY, float bX, float bY)
{
	float width = bX - aX;
	float height = bY - aY;

	float distance = sqrtf(width * width + height * height);

	float angle = acos(width / distance);

	//목표점이 더 아래에 있다면 뒤집어 주기
	if (bY > aY)
	{
		angle = PI2 - angle;
	}

	return angle;
}

/*****************************************************************
## Lerp ##
@@ start : 시작 값
@@ end : 끝 값
@@ ratio : 비율(0~1)

시작값과 끝값의 비율을 계산한 값을 반환
******************************************************************/
float Math::Lerp(float start, float end, float ratio)
{
	return start + (end - start) * ratio;
}
