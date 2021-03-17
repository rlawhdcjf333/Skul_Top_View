#pragma once
class Button final
{
	enum class State : int
	{
		Normal, Push
	};

	function<void(void)> mFunc;	//��ư�������� ����� �Լ�
	float mX;
	float mY;
	float mSizeX;
	float mSizeY;
	RECT mRect;
	State mState;
	wstring mText;
public:
	Button(wstring text,float x, float y, float sizeX, float sizeY, function<void(void)> func);

	void Update();
	void Render(HDC hdc);
};

