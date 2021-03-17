#include "pch.h"
#include "Gizmo.h"

Gizmo::Gizmo()
{
	mNullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

	HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	HPEN greenPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HPEN bluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	HPEN MargentaPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 255));

	mPenList.insert(make_pair(Color::Red, redPen));
	mPenList.insert(make_pair(Color::Green, greenPen));
	mPenList.insert(make_pair(Color::Blue, bluePen));
	mPenList.insert(make_pair(Color::Margenta, MargentaPen));
}

Gizmo::~Gizmo()
{
	PenIter iter = mPenList.begin(); 
	for (; iter != mPenList.end(); ++iter)
	{
		DeleteObject(iter->second);
	}
	DeleteObject(mNullBrush);
}

void Gizmo::DrawRect(HDC hdc, RECT rc, Color color)
{
	HPEN pen = mPenList[color];

	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc,mNullBrush);
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);
	CAMERA->RenderRect(hdc, rc);
	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
}

void Gizmo::DrawDiam (HDC hdc, Diam diam, Color color)
{
	HPEN pen = mPenList[color];

	POINT points[4] = { diam.top, diam.right, diam.bottom, diam.left };
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, mNullBrush);
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);
	CAMERA->RenderPolygon(hdc, points, 4);
	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
}

void Gizmo::DrawEllipse(HDC hdc, float x, float y, float radius, Color color)
{
	HPEN pen = mPenList[color];

	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, mNullBrush);
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);
	RenderEllipse(hdc, x,y,radius);
	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
}
