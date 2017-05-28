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
	Shape(int x, int y);
	~Shape();

	void setEndPos(int x, int y);

	virtual void draw(HDC hdc) = 0;
};

