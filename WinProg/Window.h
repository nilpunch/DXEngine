#pragma once
#include "ErrorLogger.h"
#include <stf/Vector.h>

class WindowContainer;

class Window
{
public:
	bool Initialise(WindowContainer* windowContainer, HINSTANCE hInstance, const std::wstring& title, const std::wstring& className, stf::iVec2 size, stf::iVec2 position);
	bool ProcessMessages();
	HWND GetHWND() const;
	void SetTitle(const std::wstring& title);

	int Width() const { return this->size.x; }
	int Height() const { return this->size.y; }

	~Window();
private:
	void RegisterWindowClass();
	HINSTANCE hInstance = NULL;
	HWND hWnd = NULL;
	std::wstring title;
	std::wstring className;
	stf::iVec2 size;
};

