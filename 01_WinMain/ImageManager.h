#pragma once
/*
이미지 리소스 관리하는 매니저님
*/

class Image;

class ImageManager
{
	Singleton(ImageManager)
private:
	map<wstring, Image*> mImageList;
	//typedef : 다른 이름으로도 부를 수 있게 해주는 녀석
	typedef map<wstring, Image*>::iterator ImageIter;
public:
	~ImageManager();

	void LoadFromFile(wstring key, wstring filePath, int width, int height,
		bool isTrans, COLORREF transColor = RGB(255,0,255));
	void LoadFromFile(wstring key, wstring filePath, int width, int height,
		int frameX, int frameY, bool isTrans, COLORREF transColor = RGB(255, 0, 255));

	Image* FindImage(wstring key);
};

#define IMAGEMANAGER ImageManager::GetInstance()