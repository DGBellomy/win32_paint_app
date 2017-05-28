#pragma once
#include "Shape.h"
class Line :
	public Shape
{
public:
	Line(int x, int y);
	~Line();

	virtual void draw(HDC hdc);
};

