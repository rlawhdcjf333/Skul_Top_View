#pragma once
class CameraManager
{
	Singleton(CameraManager)
private:
	class Camera* mMainCamera;
public:
	void Update();

	inline void SetMainCamera(Camera* main) { mMainCamera = main; }
	inline class Camera* GetMainCamera()const { return mMainCamera; }
};

