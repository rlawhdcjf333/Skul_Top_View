#pragma once

#define PI 3.14159265f
#define PI2 (3.14159265f * 2.f)

class Math
{
public :
	static float GetDistance(float aX, float aY, float bX, float bY);
	static float GetAngle(float aX, float aY, float bX, float bY);
	static float Lerp(float start, float end, float ratio);
};

