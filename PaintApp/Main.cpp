#include "PaintApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	PaintApp paintApp(hInstance);

	if (!paintApp.init())
	{
		MessageBox(0, L"Initialization Failed", 0, MB_OK);
		return false;
	}

	return paintApp.run();
}