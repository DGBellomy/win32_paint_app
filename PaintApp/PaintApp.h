#pragma once

#include <Windows.h>
#include <vector>
#include "Shape.h"

class PaintApp
{
public:
	enum ShapeType {LINE, BOX, OVAL};

private:
	COLORREF mPenColor;
	UINT mPenStyle;
	UINT mPenThickness;
	COLORREF mBrushColor;
	UINT mBrushStyle;
	LONG mBrushPattern;

private: // Menu IDs
	UINT mShapeID;
	UINT mPenColorID;
	UINT mPenStyleID;
	UINT mPenThicknessID;
	UINT mBrushColorID;
	UINT mBrushStyleID;
	UINT mBrushPatternID;

private: // Handles
	static PaintApp* instance;

	HWND mhMainWnd;
	HINSTANCE mhAppInst;
	HMENU mhMenu;

private: // Save State Variables
	std::vector<Shape*> mShapes;
	Shape* mCurrentShape;
	bool mMousePressed;
	ShapeType mShapeType;

public:
	PaintApp(HINSTANCE hInstance);
	~PaintApp();

	static PaintApp* getInstance();

	bool init();
	int run();
	LRESULT msgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private: // helper functions
	int setup();
	int mouseButtonDown(int x, int y);
	int mouseMove(int x, int y);
	int mouseButtonUp(int x, int y);
	int draw();
	int menuHandler(int menuItem);
	void menuIDChecker(UINT& id, UINT menuItem);
};
