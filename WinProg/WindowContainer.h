#pragma once
#include "Window.h"
#include "Keyboard/Keyboard.h"
#include "Mouse/Mouse.h"
#include "Graphics/Graphics.h"

#include <mutex>

class WindowContainer
{
public:
	WindowContainer();
	LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	Window window;
	Keyboard keyboard;
	Mouse mouse;
	Graphics gfx;
	std::mutex inputLock;
};