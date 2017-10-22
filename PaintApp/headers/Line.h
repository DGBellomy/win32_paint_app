#pragma once
#include "Shape.h"
class Line :
	public Shape
{
public:
	Line(int x, int y, HPEN hPen, HBRUSH hBrush);
	~Line();

protected:
	virtual void render(HDC hdc);
};

