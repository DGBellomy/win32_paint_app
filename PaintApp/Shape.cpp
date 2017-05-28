#include "Shape.h"



Shape::Shape(int x, int y)
{
	mStartPos.x = x;
	mStartPos.y = y;
	mEndPos.x = 0;
	mEndPos.y = 0;
}

Shape::~Shape()
{
}

void Shape::setEndPos(int x, int y)
{
	mEndPos.x = x;
	mEndPos.y = y;
}
