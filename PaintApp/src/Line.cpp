#include "Line.h"



Line::Line(int x, int y, HPEN hPen, HBRUSH hBrush)
	:Shape(x, y, hPen, hBrush)
{
}


Line::~Line()
{
}

void Line::render(HDC hdc)
{
	MoveToEx(hdc, mStartPos.x, mStartPos.y, 0);
	LineTo(hdc, mEndPos.x, mEndPos.y);
}
