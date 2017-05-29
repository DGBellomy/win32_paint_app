#pragma once

#include <Windows.h>

class Shape
{
protected:
	HPEN mhPen;
	HBRUSH mhBrush;
	POINT mStartPos;
	POINT mEndPos;
	
public:
	Shape(int x, int y, HPEN hPen, HBRUSH hBrush);
	~Shape();

	void setEndPos(int x, int y);

	void draw(HDC hdc);

protected:
	virtual void render(HDC hdc) = 0;
};

