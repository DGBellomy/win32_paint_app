#include "Shape.h"



Shape::Shape(int x, int y, HPEN hPen, HBRUSH hBrush)
	: mhPen(hPen), mhBrush(hBrush)
{
	mStartPos.x = x;
	mStartPos.y = y;
	mEndPos.x = 0;
	mEndPos.y = 0;
}

Shape::~Shape()
{
	DeleteObject(mhPen);
	DeleteObject(mhBrush);
}

void Shape::setEndPos(int x, int y)
{
	mEndPos.x = x;
	mEndPos.y = y;
}

void Shape::draw(HDC hdc)
{
	HPEN hOldPen = (HPEN)SelectObject(hdc, mhPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, mhBrush);

	render(hdc);

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
}
