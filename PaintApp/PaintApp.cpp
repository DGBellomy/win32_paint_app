#include <cassert>
#include "PaintApp.h"
#include "Line.h"


PaintApp* PaintApp::instance = nullptr;

LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return PaintApp::getInstance()->msgHandler(hWnd, msg, wParam, lParam);
}


PaintApp::PaintApp(HINSTANCE hInstance)
	: mhAppInst(hInstance), mMousePressed(false)
{
	assert(instance == nullptr);
	instance = this;
}


PaintApp::~PaintApp()
{
}

bool PaintApp::init()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"PaintApp";

	RegisterClass(&wc);

	mhMainWnd = CreateWindow(L"PaintApp", L"Paint App", WS_OVERLAPPEDWINDOW,
		0, 0,
		500, 500,
		0, 0, mhAppInst, 0);

	if (mhMainWnd == 0)
	{
		MessageBox(0, L"Create Window Failed", 0, MB_OK);
	}

	ShowWindow(mhMainWnd, SW_SHOWDEFAULT);
	UpdateWindow(mhMainWnd);

	return true;
}

int PaintApp::run()
{
	MSG msg = {0};

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

PaintApp * PaintApp::getInstance()
{
	return instance;
}

LRESULT PaintApp::msgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		// TODO: do something for setup
		return 0;
	case WM_COMMAND:
		// TODO: menu controls
		return 0;
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
		return mouseButtonDown(LOWORD(lParam), HIWORD(lParam));
	case WM_MOUSEMOVE:
		return mouseMove(LOWORD(lParam), HIWORD(lParam));
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
		return mouseButtonUp(LOWORD(lParam), HIWORD(lParam));
	case WM_PAINT:
		return draw();
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

int PaintApp::mouseButtonDown(int x, int y)
{
	mMousePressed = true;
	SetCapture(mhMainWnd);
	mCurrentShape = new Line(x, y);
	return 0;
}

int PaintApp::mouseMove(int x, int y)
{
	if (mMousePressed)
	{
		mCurrentShape->setEndPos(x, y);
		InvalidateRect(mhMainWnd, 0, true);
	}
	return 0;
}

int PaintApp::mouseButtonUp(int x, int y)
{
	mMousePressed = false;
	ReleaseCapture();
	mCurrentShape->setEndPos(x, y);
	mShapes.push_back(mCurrentShape);
	InvalidateRect(mhMainWnd, 0, true);
	return 0;
}

int PaintApp::draw()
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(mhMainWnd, &ps);

	if (mMousePressed)
	{
		mCurrentShape->draw(hdc);
	}

	for (UINT i = 0; i < mShapes.size(); i++)
	{
		mShapes[i]->draw(hdc);
	}

	EndPaint(mhMainWnd, &ps);
	return 0;
}
