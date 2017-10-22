#pragma once
#include "Shape.h"
class Rect :
	public Shape
{
public:
	Rect(int x, int y, HPEN hPen, HBRUSH hBrush);
	~Rect();

protected:
	virtual void render(HDC hdc);
};

