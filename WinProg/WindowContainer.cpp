#include "WindowContainer.h"

WindowContainer::WindowContainer()
{
	static bool isRawInputInitialised = false;
	if (isRawInputInitialised == false)
	{
		RAWINPUTDEVICE rid[1];
		// Mouse
		rid[0].usUsagePage = 0x01;
		rid[0].usUsage = 0x02;
		rid[0].dwFlags = NULL;
		rid[0].hwndTarget = NULL;
		//// Pointer WIP
		//rid[1].usUsagePage = 0x01;
		//rid[1].usUsage = 0x02;
		//rid[1].dwFlags = NULL;
		//rid[1].hwndTarget = NULL;

		if (FAILED(RegisterRawInputDevices(rid, 1, sizeof(RAWINPUTDEVICE))))
		{
			ErrorLogger::Log(GetLastError(), "Failed to register raw input devices.");
			exit(-1);
		}

		isRawInputInitialised = true;
	}
}

LRESULT WindowContainer::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
		case WM_KEYDOWN:
		{
			std::lock_guard<std::mutex> lock(inputLock);
			unsigned char ch = static_cast<unsigned char>(wParam);
			const bool wasPressed = (lParam & 0x40000000) == 0x40000000;
			if (this->keyboard.IsAutoRepeatKeys())
			{
				if (wasPressed)
				{
					this->keyboard.OnKeyEvent(KeyboardEvent(KeyboardEvent::Repeat, ch));
				}
				else
				{
					this->keyboard.OnKeyEvent(KeyboardEvent(KeyboardEvent::Press, ch));
				}
			}
			else
			{
				if (!wasPressed)
				{
					this->keyboard.OnKeyEvent(KeyboardEvent(KeyboardEvent::Press, ch));
				}
			}
			return 0;
		}
		case WM_SYSKEYDOWN:
		{
			std::lock_guard<std::mutex> lock(inputLock);
			unsigned char ch = static_cast<unsigned char>(wParam);
			const bool wasPressed = (lParam & 0x40000000) == 0x40000000;
			if (this->keyboard.IsAutoRepeatKeys())
			{
				if (wasPressed)
				{
					this->keyboard.OnKeyEvent(KeyboardEvent(KeyboardEvent::Repeat, ch));
				}
				else
				{
					this->keyboard.OnKeyEvent(KeyboardEvent(KeyboardEvent::Press, ch));
				}
			}
			else
			{
				if (!wasPressed)
				{
					this->keyboard.OnKeyEvent(KeyboardEvent(KeyboardEvent::Press, ch));
				}
			}
			return 0;
		}
		case WM_SYSKEYUP:
		{
			std::lock_guard<std::mutex> lock(inputLock);
			unsigned char ch = static_cast<unsigned char>(wParam);
			this->keyboard.OnKeyEvent(KeyboardEvent(KeyboardEvent::Release, ch));
			return 0;
		}
		case WM_KEYUP:
		{
			std::lock_guard<std::mutex> lock(inputLock);
			unsigned char ch = static_cast<unsigned char>(wParam);
			this->keyboard.OnKeyEvent(KeyboardEvent(KeyboardEvent::Release, ch));
			return 0;
		}
		case WM_CHAR:
		{
			std::lock_guard<std::mutex> lock(inputLock);
			unsigned char ch = static_cast<unsigned char>(wParam);
			if (this->keyboard.IsAutoRepeatChars())
			{
				this->keyboard.OnCharEvent(ch);
			}
			else
			{
				const bool wasPressed = (lParam & 0x40000000) == 0x40000000;
				if (!wasPressed)
				{
					this->keyboard.OnCharEvent(ch);
				}
			}
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			std::lock_guard<std::mutex> lock(inputLock);
			this->mouse.OnMoveEvent({ LOWORD(lParam), HIWORD(lParam) });
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			std::lock_guard<std::mutex> lock(inputLock);
			this->mouse.OnButtonEvent(MouseEvent(MouseEvent::Press, { LOWORD(lParam), HIWORD(lParam) }, BUTTON::LEFT));
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			std::lock_guard<std::mutex> lock(inputLock);
			this->mouse.OnButtonEvent(MouseEvent(MouseEvent::Press, { LOWORD(lParam), HIWORD(lParam) }, BUTTON::RIGHT));
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			std::lock_guard<std::mutex> lock(inputLock);
			this->mouse.OnButtonEvent(MouseEvent(MouseEvent::Press, { LOWORD(lParam), HIWORD(lParam) }, BUTTON::MIDDLE));
			return 0;
		}
		case WM_LBUTTONUP:
		{
			std::lock_guard<std::mutex> lock(inputLock);
			this->mouse.OnButtonEvent(MouseEvent(MouseEvent::Release, { LOWORD(lParam), HIWORD(lParam) }, BUTTON::LEFT));
			return 0;
		}
		case WM_RBUTTONUP:
		{
			std::lock_guard<std::mutex> lock(inputLock);
			this->mouse.OnButtonEvent(MouseEvent(MouseEvent::Release, { LOWORD(lParam), HIWORD(lParam) }, BUTTON::RIGHT));
			return 0;
		}
		case WM_MBUTTONUP:
		{
			std::lock_guard<std::mutex> lock(inputLock);
			this->mouse.OnButtonEvent(MouseEvent(MouseEvent::Release, { LOWORD(lParam), HIWORD(lParam) }, BUTTON::MIDDLE));
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			std::lock_guard<std::mutex> lock(inputLock);
			this->mouse.OnWheelEvent(GET_WHEEL_DELTA_WPARAM(wParam), { LOWORD(lParam), HIWORD(lParam) });
			return 0;
		}
		case WM_INPUT:
		{
			std::lock_guard<std::mutex> lock(inputLock);
			UINT dataSize;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
			if (dataSize > 0)
			{
				std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);

				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
				{
					RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
					if (raw->header.dwType == RIM_TYPEMOUSE)
					{
						this->mouse.OnRawInput({ raw->data.mouse.lLastX, raw->data.mouse.lLastY });
					}
				}
			}
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}