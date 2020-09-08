//#include "DirectXEngine.h"
//#define DXE_DEBUG
//#define DXE_NO_FPS_LIMIT
#include "DXEngine.h"



class DemoWindow : public DXEngine
{
protected:
	virtual void Init() override
	{
		SetFramerate(60);
		SetTitle(L"Test");
	}

	bool color_switch = false;

	int Framerate = 60;

	virtual void Update() override
	{
		color_switch = !color_switch;
		if (color_switch)
			ClearScreen({ 0.1f, 0.1f, 0.5f, 1.0f });
		else
			ClearScreen({ 0.1f, 0.5f, 0.1f, 1.0f });

		if (GetKey(KEY::ESC).Pressed)
		{
			Quit();
		}
	}
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	DemoWindow we;

	if (!we.ConstructWindowClass(hInstance, lpCmdLine, nShowCmd))
	{
		MessageBox(NULL, L"WNDCLASSEX construction failure.", L"DXEngine Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	if (!we.ConstructWindow(200, 200, 500, 500))
	{
		MessageBox(NULL, L"Window construction failure.", L"DXEngine Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	we.Start();

	return we.End().wParam;
}