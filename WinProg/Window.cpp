#include "WindowContainer.h"

bool Window::Initialise(WindowContainer* wndContainer, HINSTANCE hInstance, const std::wstring& title, const std::wstring& className, stf::iVec2 size, stf::iVec2 position = {0, 0})
{
	this->hInstance = hInstance;
	this->title = title;
	this->className = className;
	this->size = size;

	this->RegisterWindowClass();

	RECT wr;
	wr.left = position.x;
	wr.top = position.y;
	wr.right = wr.left + size.x;
	wr.bottom = wr.top + size.y;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	this->hWnd = CreateWindowEx(NULL, className.c_str(), title.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, wr.left, wr.top, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, hInstance, wndContainer);

	if (this->hWnd == NULL)
	{
		ErrorLogger::Log(GetLastError(), "Failed to create window.");
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	return true;
}

bool Window::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(hWnd))
		{
			hWnd = NULL;
			UnregisterClass(className.c_str(), hInstance);
			return false;
		}
	}
	return true;
}

HWND Window::GetHWND() const
{
	return this->hWnd;
}

Window::~Window()
{
	if (this->hWnd != NULL)
	{
		UnregisterClass(this->className.c_str(), this->hInstance);
		DestroyWindow(this->hWnd);
	}
}

void Window::SetTitle(const std::wstring & title)
{
	this->title = title;
	SetWindowText(this->hWnd, this->title.c_str());
}

LRESULT CALLBACK HandleMessageRedirect(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			return 0;
		}

		default:
		{
			WindowContainer* windowContainer = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			return windowContainer->WindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_NCCREATE:
		{
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			WindowContainer* windowContainer = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
			if (windowContainer == nullptr)
			{
				ErrorLogger::Log("Critical Error: Pointer to window container is null");
				exit(-1);
			}
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowContainer));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMessageRedirect));
			return windowContainer->WindowProc(hWnd, uMsg, wParam, lParam);
		}

		default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
}

void Window::RegisterWindowClass()
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.hInstance = this->hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = className.c_str();
	RegisterClassEx(&wc);
}
