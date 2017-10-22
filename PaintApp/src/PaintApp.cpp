#include <cassert>
#include "PaintApp.h"
#include "Line.h"
#include "Rect.h"
#include "Oval.h"
#include "resource.h"
#include "Colors.h"


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
	mhMenu = LoadMenu(mhAppInst, MAKEINTRESOURCE(IDR_MENU1));
}


PaintApp::~PaintApp()
{
	for (UINT i = 0; i < mShapes.size(); i++)
	{
		delete mShapes[i];
		mShapes[i] = 0;
	}
	mShapes.clear();
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
		0, mhMenu, mhAppInst, 0);

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
		return setup();
	case WM_COMMAND:
		return menuHandler(LOWORD(wParam));
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

int PaintApp::setup()
{
	mShapeID = ID_SHAPES_LINE;
	CheckMenuItem(mhMenu, mShapeID, MF_CHECKED);
	mPenColorID = ID_PENS_COLOR_BLACK;
	CheckMenuItem(mhMenu, mPenColorID, MF_CHECKED);
	mPenStyleID = ID_PENS_STYLE_SOLID;
	CheckMenuItem(mhMenu, mPenStyleID, MF_CHECKED);
	mPenThicknessID = ID_PENS_THICKNESS_1;
	CheckMenuItem(mhMenu, mPenThicknessID, MF_CHECKED);
	mBrushColorID = ID_BRUSHES_COLOR_BLACK;
	CheckMenuItem(mhMenu, mBrushColorID, MF_CHECKED);
	mBrushStyleID = ID_BRUSHES_STYLE_NONE;
	CheckMenuItem(mhMenu, mBrushStyleID, MF_CHECKED);
	mBrushPatternID = ID_BRUSHES_PATTERN_CROSSED;
	CheckMenuItem(mhMenu, mBrushPatternID, MF_CHECKED);

	mShapeType = LINE;

	// Initial Pen Settings
	mPenColor = PAINT_COLORS.BLACK;
	mPenThickness = 1;
	mPenStyle = PS_SOLID;

	// Initial Brush Settings
	mBrushColor = PAINT_COLORS.BLACK;
	mBrushPattern = HS_CROSS;
	mBrushStyle = BS_NULL;

	return 0;
}

int PaintApp::mouseButtonDown(int x, int y)
{
	mMousePressed = true;
	SetCapture(mhMainWnd);

	LOGPEN lp;
	lp.lopnColor = mPenColor;
	lp.lopnWidth.x = mPenThickness;
	lp.lopnWidth.y = mPenThickness;
	lp.lopnStyle = mPenStyle;

	LOGBRUSH lb;
	lb.lbColor = mBrushColor;
	lb.lbStyle = mBrushStyle;
	lb.lbHatch = mBrushPattern;

	HPEN hPen = CreatePenIndirect(&lp);
	HBRUSH hBrush = CreateBrushIndirect(&lb);

	switch (mShapeType)
	{
	case LINE:
		mCurrentShape = new Line(x, y, hPen, hBrush);
		break;
	case BOX:
		mCurrentShape = new Rect(x, y, hPen, hBrush);
		break;
	case OVAL:
		mCurrentShape = new Oval(x, y, hPen, hBrush);
		break;
	}
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
	mCurrentShape = 0;
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

int PaintApp::menuHandler(int menuItem)
{
	switch (menuItem)
	{
	case ID_FILE_EXIT:
		if (MessageBox(0, L"Are you sure you want to quit?", L"Quit Message", MB_YESNO) == IDYES)
		{
			DestroyWindow(mhMainWnd);
		}
		return 0;

	case ID_SHAPES_LINE:
		menuIDChecker(mShapeID, menuItem);
		mShapeType = LINE;
		return 0;
	case ID_SHAPES_RECTANGL:
		menuIDChecker(mShapeID, menuItem);
		mShapeType = BOX;
		return 0;
	case ID_SHAPES_ELLIPSE:
		menuIDChecker(mShapeID, menuItem);
		mShapeType = OVAL;
		return 0;

	case ID_PENS_COLOR_BLACK:
		menuIDChecker(mPenColorID, menuItem);
		mPenColor = PAINT_COLORS.BLACK;
		return 0;
	case ID_PENS_COLOR_RED:
		menuIDChecker(mPenColorID, menuItem);
		mPenColor = PAINT_COLORS.RED;
		return 0;
	case ID_PENS_COLOR_ORANGE:
		menuIDChecker(mPenColorID, menuItem);
		mPenColor = PAINT_COLORS.ORANGE;
		return 0;
	case ID_PENS_COLOR_YELLOW:
		menuIDChecker(mPenColorID, menuItem);
		mPenColor = PAINT_COLORS.YELLOW;
		return 0;
	case ID_PENS_COLOR_BLUE:
		menuIDChecker(mPenColorID, menuItem);
		mPenColor = PAINT_COLORS.BLUE;
		return 0;
	case ID_PENS_COLOR_GREEN:
		menuIDChecker(mPenColorID, menuItem);
		mPenColor = PAINT_COLORS.GREEN;
		return 0;
	case ID_PENS_COLOR_PURPLE:
		menuIDChecker(mPenColorID, menuItem);
		mPenColor = PAINT_COLORS.PURPLE;
		return 0;

	case ID_PENS_STYLE_SOLID:
		menuIDChecker(mPenStyleID, menuItem);
		mPenStyle = PS_SOLID;
		return 0;
	case ID_PENS_STYLE_DASH:
		menuIDChecker(mPenStyleID, menuItem);
		mPenStyle = PS_DASH;
		return 0;
	case ID_PENS_STYLE_DOT:
		menuIDChecker(mPenStyleID, menuItem);
		mPenStyle = PS_DOT;
		return 0;

	case ID_PENS_THICKNESS_1:
		menuIDChecker(mPenThicknessID, menuItem);
		mPenThickness = 1;
		return 0;
	case ID_PENS_THICKNESS_2:
		menuIDChecker(mPenThicknessID, menuItem);
		mPenThickness = 2;
		return 0;
	case ID_PENS_THICKNESS_3:
		menuIDChecker(mPenThicknessID, menuItem);
		mPenThickness = 3;
		return 0;
	case ID_PENS_THICKNESS_4:
		menuIDChecker(mPenThicknessID, menuItem);
		mPenThickness = 4;
		return 0;
	case ID_PENS_THICKNESS_5:
		menuIDChecker(mPenThicknessID, menuItem);
		mPenThickness = 5;
		return 0;

	case ID_BRUSHES_COLOR_BLACK:
		menuIDChecker(mBrushColorID, menuItem);
		mBrushColor = PAINT_COLORS.BLACK;
		return 0;
	case ID_BRUSHES_COLOR_RED:
		menuIDChecker(mBrushColorID, menuItem);
		mBrushColor = PAINT_COLORS.RED;
		return 0;
	case ID_BRUSHES_COLOR_ORANGE:
		menuIDChecker(mBrushColorID, menuItem);
		mBrushColor = PAINT_COLORS.ORANGE;
		return 0;
	case ID_BRUSHES_COLOR_YELLOW:
		menuIDChecker(mBrushColorID, menuItem);
		mBrushColor = PAINT_COLORS.YELLOW;
		return 0;
	case ID_BRUSHES_COLOR_BLUE:
		menuIDChecker(mBrushColorID, menuItem);
		mBrushColor = PAINT_COLORS.BLUE;
		return 0;
	case ID_BRUSHES_COLOR_GREEN:
		menuIDChecker(mBrushColorID, menuItem);
		mBrushColor = PAINT_COLORS.GREEN;
		return 0;
	case ID_BRUSHES_COLOR_PURPLE:
		menuIDChecker(mBrushColorID, menuItem);
		mBrushColor = PAINT_COLORS.PURPLE;
		return 0;

	case ID_BRUSHES_STYLE_NONE:
		menuIDChecker(mBrushStyleID, menuItem);
		mBrushStyle = BS_NULL;
		return 0;
	case ID_BRUSHES_STYLE_HATCHED:
		menuIDChecker(mBrushStyleID, menuItem);
		mBrushStyle = BS_HATCHED;
		return 0;
	case ID_BRUSHES_STYLE_SOLID:
		menuIDChecker(mBrushStyleID, menuItem);
		mBrushStyle = BS_SOLID;
		return 0;

	case ID_BRUSHES_PATTERN_CROSSED:
		menuIDChecker(mBrushPatternID, menuItem);
		mBrushPattern = HS_CROSS;
		return 0;
	case ID_BRUSHES_PATTERN_HORIZONTAL:
		menuIDChecker(mBrushPatternID, menuItem);
		mBrushPattern = HS_HORIZONTAL;
		return 0;
	case ID_BRUSHES_PATTERN_VERTICAL:
		menuIDChecker(mBrushPatternID, menuItem);
		mBrushPattern = HS_VERTICAL;
		return 0;
	}
	return 0;
}

void PaintApp::menuIDChecker(UINT & id, UINT menuItem)
{
		CheckMenuItem(mhMenu, id, MF_UNCHECKED);
		id = menuItem;
		CheckMenuItem(mhMenu, id, MF_CHECKED);
}
