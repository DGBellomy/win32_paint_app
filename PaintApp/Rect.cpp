#include "Rect.h"



Rect::Rect(int x, int y, HPEN hPen, HBRUSH hBrush)
	:Shape(x, y, hPen, hBrush)
{
}

Rect::~Rect()
{
}

void Rect::render(HDC hdc)
{
	Rectangle(hdc, mStartPos.x, mStartPos.y, mEndPos.x, mEndPos.y);
}
