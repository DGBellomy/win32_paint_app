#include "Oval.h"



Oval::Oval(int x, int y, HPEN hPen, HBRUSH hBrush)
	: Shape(x, y, hPen, hBrush)
{
}


Oval::~Oval()
{
}


void Oval::render(HDC hdc)
{
	Ellipse(hdc, mStartPos.x, mStartPos.y, mEndPos.x, mEndPos.y);
}
