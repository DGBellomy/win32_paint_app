#pragma once

#include <Windows.h>
#include <vector>
#include "Shape.h"

class PaintApp
{
private:
	static PaintApp* instance;

	HWND mhMainWnd;
	HINSTANCE mhAppInst;
	std::vector<Shape*> mShapes;
	Shape* mCurrentShape;
	bool mMousePressed;

public:
	PaintApp(HINSTANCE hInstance);
	~PaintApp();

	static PaintApp* getInstance();

	bool init();
	int run();
	LRESULT msgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	int mouseButtonDown(int x, int y);
	int mouseMove(int x, int y);
	int mouseButtonUp(int x, int y);
	int draw();
};
