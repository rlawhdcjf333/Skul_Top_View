#include "Item.h"

class HeartOfNaias :public Item
{
	float mValue;

public:

	HeartOfNaias(int indexX, int indexY);
	void Update();
	void Render(HDC hdc);
	void Release();
};





