#include "Line.h"



Line::Line(int x, int y)
	:Shape(x, y)
{
}


Line::~Line()
{
}

void Line::draw(HDC hdc)
{
	MoveToEx(hdc, mStartPos.x, mStartPos.y, 0);
	LineTo(hdc, mEndPos.x, mEndPos.y);
}
