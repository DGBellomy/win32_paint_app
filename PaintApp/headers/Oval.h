#pragma once
#include "Shape.h"
class Oval :
	public Shape
{
public:
	Oval(int x, int y, HPEN hPen, HBRUSH hBrush);
	~Oval();

protected:
	virtual void render(HDC hdc);
};

