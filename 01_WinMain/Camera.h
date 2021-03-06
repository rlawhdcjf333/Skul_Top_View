#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public:
	enum class Mode : int
	{
		Follow, Free
	};
protected:
	Mode mMode;
	GameObject* mTarget;		//?Ѿư????? Ÿ??

	float mMoveSpeed;

	int mPanningPower;
	float mPanningTime = 0.5f;
	bool mIsPanning = false;

public:
	void Init()override; 
	void Release()override; 
	void Update()override; 
	void Render(HDC hdc)override;

	void SetTarget(GameObject* target) { mTarget = target; }
	void ChangeMode(Mode mode) { mMode = mode; }
public:
	void Render(HDC hdc, class Image* image, int x, int y);
	void Render(HDC hdc, class Image* image, int x, int y, int tempX, int tempY, int tempWidth, int tempHeight);
	void FrameRender(HDC hdc, class Image* image, int x, int y, int frameX, int frameY);
	void AlphaRender(HDC hdc, class Image* image, int x, int y, float alpha);
	void AlphaFrameRender(HDC hdc, class Image* image, int x, int y, int frameX, int frameY, float alpha);
	void ScaleRender(HDC hdc, class Image* image, int x, int y, int width, int height);
	void ScaleFrameRender(HDC hdc,class Image* image, int x, int y, int frameX, int frameY, int width, int height);
	void AlphaScaleRender(HDC hdc,class Image* image, int x, int y, int width, int height, float alpha);
	void AlphaScaleFrameRender(HDC hdc, class Image* image, int x, int y, int frameX, int frameY, int width, int height, float alpha);
	void CenterBottomFrameRender(HDC hdc , class Image* image, int x, int y, int frameX, int frameY);
	void RenderRect(HDC hdc, RECT rc);
	void RenderPolygon(HDC hdc, POINT points[], int pointsNum);
	void RenderEllipse(HDC hdc, float x, float y, float radius);

	bool IsInCameraArea(float x, float y, float width, float height);
	bool IsInCameraArea(RECT rc);

	inline float GetMoveSpeed()const { return mMoveSpeed; }
	inline void SetMoveSpeed(float speed) { mMoveSpeed = speed; }

	void Panning();
	void PanningOn(int power) { mPanningPower = power;  mIsPanning = true; }

	LONG CameraMouseX();
	LONG CameraMouseY();
};