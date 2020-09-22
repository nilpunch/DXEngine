#pragma once
#pragma once
#include "Window.h"
#include "WindowContainer.h"
#include "ErrorLogger.h"


#pragma comment(lib, "winmm.lib")

#ifndef UNICODE
#error Please enable UNICODE for your compiler! VS: Project Properties -> General -> \
Character Set -> Use Unicode.
#endif

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

// Include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include <string>
#include <vector>
#include <array>
#include <assert.h>

#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

#include <stf/Random.h>
#include <stf/Vector.h>
#include <stf/Matrix.h>

#define PI 3.141592653589793
#define TWO_PI 6.283185307179586
#define HALF_PI 1.570796326794896

// #define WE_NO_FPS_LIMIT // Uncomment this to unlock FPS
#define DXE_DEFAULT_FPS_LIM 60.0
#define DXE_MOUSE_MAX_BUTTONS 5
#define DXE_AVERAGE_FRAMELIST_SIZE 10

// In case this is wheel roll up and down event
#define DXE_MOUSE_ADDITIONAL_EVENTS 2

// In this namespace defined a lot of cool (my own) usefull classes 
using namespace stf;

// Using time literals
using namespace std::literals::chrono_literals;

// Character color attributes
enum COLOR : short
{
	FG_BLACK = 0x0000, BG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001, BG_DARK_BLUE = 0x0010,
	FG_DARK_GREEN = 0x0002, BG_DARK_GREEN = 0x0020,
	FG_DARK_CYAN = 0x0003, BG_DARK_CYAN = 0x0030,
	FG_DARK_RED = 0x0004, BG_DARK_RED = 0x0040,
	FG_DARK_MAGENTA = 0x0005, BG_DARK_MAGENTA = 0x0050,
	FG_DARK_YELLOW = 0x0006, BG_DARK_YELLOW = 0x0060,
	FG_GREY = 0x0007, BG_GREY = 0x0070,
	FG_DARK_GREY = 0x0008, BG_DARK_GREY = 0x0080,
	FG_BLUE = 0x0009, BG_BLUE = 0x0090,
	FG_GREEN = 0x000A, BG_GREEN = 0x00A0,
	FG_CYAN = 0x000B, BG_CYAN = 0x00B0,
	FG_RED = 0x000C, BG_RED = 0x00C0,
	FG_MAGENTA = 0x000D, BG_MAGENTA = 0x00D0,
	FG_YELLOW = 0x000E, BG_YELLOW = 0x00E0,
	FG_WHITE = 0x000F, BG_WHITE = 0x00F0
};

// UNICODE characters with filled quads
enum QUAD : short
{
	SOLID = 0x2588,
	THREEQUARTERS = 0x2593,
	HALF = 0x2592,
	QUARTER = 0x2591
};

// UNICODE characters for draw command-line shapes
enum BOXSHAPE : short
{
	L_UP = 0x250c, R_UP = 0x2510,
	L_DOWN = 0x2514, R_DOWN = 0x2518,
	L_UP2 = 0x2554, R_UP2 = 0x2557,
	L_DOWN2 = 0x255a, R_DOWN2 = 0x255d,
	HOR = 0x2500, VER = 0x2502,
	HOR2 = 0x2550, VER2 = 0x2551,
	T_R = 0x251c, T_L = 0x2524, T_D = 0x252c, T_U = 0x2534,
	T_R2 = 0x2560, T_L2 = 0x2563, T_D2 = 0x2566, T_U2 = 0x2569,
	B_CROSS = 0x2573,
	A_U = 0x25b2, A_D = 0x25bc, A_L = 0x25c4, A_R = 0x25ba
};

// Keyboard virtual-key codes
enum class KEY : size_t
{
	EMPTY = 0x40,
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	K0 = 0x30, K1, K2, K3, K4, K5, K6, K7, K8, K9,
	F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	LEFT = 0x25, UP, RIGHT, DOWN,
	SPACE = 0x20, PAGE_UP, PAGE_DOWN, END, HOME, TAB = 0x09, LSHIFT = 0xA0, RSHIFT, LCTRL, RCTRL, INS = 0x2D, DEL,
	BACKSPACE = 0x08, ESC = 0x1B, ENTER = 0x0D, CTRL, ALT, PAUSE,
	NP0 = 0x60, NP1, NP2, NP3, NP4, NP5, NP6, NP7, NP8, NP9,
	NP_MUL, NP_ADD, NP_SEPARATOR, NP_SUB, NP_DECIMAL
};

// Mouse buttons virtual-key codes
enum class BUTTON : size_t
{
	LEFT = 0x0,
	RIGHT = 0x1,
	MIDDLE = 0x2,
	M1, M2, M3,
	// Last two constant reserved for wheel registration
	WH_FORWARD = DXE_MOUSE_MAX_BUTTONS + 0x0,
	WH_BACKWARD = DXE_MOUSE_MAX_BUTTONS + 0x1
};

// Cursor images enumeration
enum class CURSOR : short
{
	ARROW = 0x0,
	ARROW_WATCH,
	WAIT_WATCH,
	CROSS,
	TEXT,
	SIZE_WIDTH,
	SIZE_HEIGHT,
	SIZE_DIAGONAL_LEFT,
	SIZE_DIAGONAL_RIGHT,
	SIZE_ALL,
	HAND,
	PIN
};

// Struct that handle state of button
struct KeyState
{
	bool Pressed = false;
	bool Released = false;
	bool Held = false;
};

// Struct that handle keys update info & states between frames
struct KeyInfo
{
	union
	{
		size_t Code;
		KEY Key = KEY::EMPTY;
	};
	KeyState State;
};

/* You must publicly inheritated from this */
class DXEngine : WindowContainer
{
public:
	bool InitialiseWindow(HINSTANCE hInstance, const std::wstring& title, iVec2 size, iVec2 position = iVec2{ 0, 0 }, const std::wstring& className = L"WindowClass1")
	{
		return this->window.Initialise(this, hInstance, title, className, size, position);
	}
	bool ProcessMessages()
	{
		return this->window.ProcessMessages();
	}


	/* Drawing routine */
private:
	IDXGISwapChain *m_Swapchain;   // The pointer to the swap chain interface
	ID3D11Device *m_Device;           // The pointer to our Direct3D m_Device interface
	ID3D11DeviceContext *m_Devcon; // The pointer to our Direct3D m_Device context

	ID3D11RenderTargetView *m_Backbuffer; // Backbuffer

public:
	constexpr IDXGISwapChain* const Swapchain() { return m_Swapchain; }
	constexpr ID3D11Device* const Device() { return m_Device; }
	constexpr ID3D11DeviceContext* const Devcon() { return m_Devcon; }
	constexpr ID3D11RenderTargetView* const Backbuffer() { return m_Backbuffer; }

public:
	// Clear the back vertexBuffer
	void ClearScreen(const D3DXCOLOR& color)
	{
		m_Devcon->ClearRenderTargetView(m_Backbuffer, color);
	}

protected:
	/* Must override this */
	virtual void Init() = 0;				// Proc once on start of program                     
	virtual void Update() = 0;				// Run code in function with FPS lock, must handle drawing routine

	/* Not necessary to override */
	virtual void Destroy() {}		// Proc once on exit from Update()

public:
	void Start()
	{
		// Initialise D3D
		InitD3D();

		// Setup main thread & application start time point
		DXE_MainThreadActive = true;
		m_tpStartProgram = std::chrono::system_clock::now();
		std::thread thMainLoop = std::thread(&DXEngine::MainLoopThread, this);

		// Start message loop in main thread
		WndMessageLoopThread();
		// Join game loop in separate thread
		thMainLoop.join();

		// Clenup after exit from loop
		CleanD3D();
	}

	void Quit()
	{
		DXE_MainThreadActive = false;
	}

	const MSG& End()
	{
		return m_msg;
	}

	/* Main engine part */
private:
	void WndMessageLoopThread()
	{
		while (DXE_MainThreadActive)
		{
			if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&m_msg);
				DispatchMessage(&m_msg);

				if (m_msg.message == WM_QUIT)
				{
					DXE_MainThreadActive = false;
					Destroy();
				}
			}


		}
	}

	MSG m_msg;
	void MainLoopThread()
	{
		// Initialise game
		Init();

#ifndef DXE_NO_FPS_LIMIT
		// Auto FPS lock
		auto tpBeginFrame = std::chrono::high_resolution_clock::now();
		auto tpEndFrame = tpBeginFrame + m_FPS;
#endif
		// Handle FPS
		auto tpPrevTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
		std::chrono::duration<double> tpElapsedTime;

		while (DXE_MainThreadActive)
		{
			// Update keystates
			UpdateTemporaryInputSystem();

			// Update the game states
			Update();

#ifndef DXE_NO_FPS_LIMIT
			auto tpCurrentTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
			// Stable FPS
			if (tpCurrentTime < tpEndFrame)
			{
				std::this_thread::sleep_until(tpEndFrame);
				tpBeginFrame = tpEndFrame;
			}
			else
			{
				tpBeginFrame = tpCurrentTime;
			}
			tpEndFrame = tpBeginFrame + m_FPS;
#endif

			// Handle framerate routine
			tpElapsedTime = tpCurrentTime - tpPrevTime;
			tpPrevTime = tpCurrentTime;
			m_StableDeltaTime = tpElapsedTime.count();

			// Switch the back vertexBuffer and the front vertexBuffer
			m_Swapchain->Present(0, 0);
		}
	}

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			} break;
			case WM_KEYUP: case WM_KEYDOWN:
			{
				HandleKeysMessages(message, wParam, lParam);
				return 0;
			} break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	static void HandleKeysMessages(UINT message, WPARAM wParam, LPARAM lParam)
	{
	}

	void UpdateTemporaryInputSystem()
	{
		// Handle Keyboard Input
		for (size_t key_i = 0; key_i < 256; ++key_i)
		{
			m_keysNewState[key_i] = GetAsyncKeyState(key_i);
			m_Keys[key_i].Pressed = false;
			m_Keys[key_i].Released = false;
			if (m_keysNewState[key_i] != m_keysOldState[key_i])
			{
				m_LastKey = (KEY)key_i;
				if (m_keysNewState[key_i] & 0x8000)
				{
					m_Keys[key_i].Pressed = !m_Keys[key_i].Held;
					m_Keys[key_i].Held = true;
				}
				else
				{
					m_Keys[key_i].Released = true;
					m_Keys[key_i].Held = false;
				}
			}
			m_keysOldState[key_i] = m_keysNewState[key_i];
		}
	}

	void InitD3D()
	{
		// Create a struct to hold information about the swap chain
		DXGI_SWAP_CHAIN_DESC scd;

		// Clear out the struct for use
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		// Fill the swap chain description struct
		scd.BufferCount = 1;                                // One back vertexBuffer
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Use 32-bit color
		scd.BufferDesc.Width = m_Screen.x;					// Set the back vertexBuffer width
		scd.BufferDesc.Height = m_Screen.y;					// Set the back vertexBuffer height
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// How swap chain is to be used
		scd.OutputWindow = m_hWnd;                          // The window to be used
		scd.SampleDesc.Count = 4;                           // How many multisamples
		scd.Windowed = TRUE;                                // Windowed/full-screen mode
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// Create a m_Device, m_Device context and swap chain using the information in the scd struct
		D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &m_Swapchain, &m_Device, NULL, &m_Devcon);

		// Get the address of the back vertexBuffer
		ID3D11Texture2D *pBackBuffer;
		m_Swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// Use the back vertexBuffer address to create the render target
		m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_Backbuffer);
		pBackBuffer->Release();

		// Set the render target as the back vertexBuffer
		m_Devcon->OMSetRenderTargets(1, &m_Backbuffer, NULL);

		// Set the viewport
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)m_Screen.x;
		viewport.Height = (float)m_Screen.y;

		m_Devcon->RSSetViewports(1, &viewport);
	}

	// This is the function that cleans up Direct3D and COM
	void CleanD3D()
	{
		// Switch to windowed mode
		m_Swapchain->SetFullscreenState(FALSE, NULL);

		// Close and release all existing COM objects
		m_Swapchain->Release();
		m_Backbuffer->Release();
		m_Device->Release();
		m_Devcon->Release();
	}

	/* Screen info */
private:
	iVec2 m_Screen;

public:
	constexpr const int ScreenWidth() const
	{
		return m_Screen.x;
	}
	constexpr const int ScreenHeight() const
	{
		return m_Screen.y;
	}
	const iVec2 Screen() const
	{
		return m_Screen;
	}

	/* Input */
private:
	KeyState m_Keys[256], m_Mouse[DXE_MOUSE_MAX_BUTTONS + DXE_MOUSE_ADDITIONAL_EVENTS];
	KEY m_LastKey = KEY::EMPTY;
	std::vector<KeyInfo> m_KeyBuffer;

	int m_MouseX;
	int m_MouseY;

public:
	constexpr const int& GetMouseX() const
	{
		return m_MouseX;
	}
	constexpr const int& GetMouseY() const
	{
		return m_MouseY;
	}
	constexpr const KeyState& GetMouseButton(BUTTON mouseButton) const
	{
		return m_Mouse[(size_t)mouseButton];
	}
	iVec2 GetMouse() const
	{
		return iVec2{ m_MouseX, m_MouseY };
	}

	/*void SetMouseCursor(CURSOR cursorType)
	{
		LPCWSTR CursorName = IDC_ARROW;
		switch (cursorType)
		{
			case CURSOR::ARROW:					CursorName = IDC_ARROW; break;
			case CURSOR::ARROW_WATCH:			CursorName = IDC_APPSTARTING; break;
			case CURSOR::WAIT_WATCH:			CursorName = IDC_WAIT; break;
			case CURSOR::CROSS:					CursorName = IDC_CROSS; break;
			case CURSOR::TEXT:					CursorName = IDC_IBEAM; break;
			case CURSOR::SIZE_WIDTH:			CursorName = IDC_SIZEWE; break;
			case CURSOR::SIZE_HEIGHT:			CursorName = IDC_SIZENS; break;
			case CURSOR::SIZE_DIAGONAL_LEFT:	CursorName = IDC_SIZENESW; break;
			case CURSOR::SIZE_DIAGONAL_RIGHT:	CursorName = IDC_SIZENWSE; break;
			case CURSOR::SIZE_ALL:				CursorName = IDC_SIZEALL; break;
			case CURSOR::HAND:					CursorName = IDC_HAND; break;
			case CURSOR::PIN:					CursorName = IDC_PIN; break;
		}

		m_hCursor = LoadCursor(NULL, CursorName);
		SetCursor(m_hCursor);
	}*/

	constexpr const KeyState& GetKey(KEY key) const
	{
		return m_Keys[(size_t)key];
	}
	constexpr const KEY& GetLastKeyCode() const
	{
		return m_LastKey;
	}
	constexpr const KeyState& GetLastKey() const
	{
		return m_Keys[(size_t)m_LastKey];
	}

	/* Timing things */
private:
	std::chrono::duration<long long, std::ratio_multiply<std::hecto, std::nano>> m_FPS =
		std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<double>{ 1.0 / DXE_DEFAULT_FPS_LIM });
	double m_StableDeltaTime = 0.0f;
	std::chrono::system_clock::time_point m_tpStartProgram;

public:
	// Return elapsed time between since the last frame
	constexpr const double& DeltaTime() const
	{
		return m_StableDeltaTime;
	}

	// Return elapsed time since the start of the program in ms
	constexpr double RunTime() const
	{
		return std::chrono::duration<double>(std::chrono::system_clock::now() - m_tpStartProgram).count();
	}

	void SetFramerate(double FPS)
	{
		m_FPS = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<double>{ 1.0 / FPS });
	}

private:
	std::wstring m_WindowTitle;

	HCURSOR m_hCursor;

	uint16_t m_keysOldState[256] = { 0 };
	uint16_t m_keysNewState[256] = { 0 };

	// WIP
	/*bool m_mouseOldState[DXE_MOUSE_MAX_BUTTONS + DXE_MOUSE_ADDITIONAL_EVENTS] = { 0 };
	bool m_mouseNewState[DXE_MOUSE_MAX_BUTTONS + DXE_MOUSE_ADDITIONAL_EVENTS] = { 0 };*/

	static std::atomic<bool> DXE_MainThreadActive;
};

DXEngine* DXEngine::Instance = nullptr;
std::atomic<bool> DXEngine::DXE_MainThreadActive(false);

//------- Functions to work with DXEngine -----------------------------------------------------------
namespace DXE
{
	class Shader
	{
	private:
		ID3D11VertexShader *m_VShader = nullptr;
		ID3D11PixelShader *m_PShader = nullptr;
		ID3D10Blob *m_VBlob = nullptr;
		ID3D10Blob *m_PBlob = nullptr;

	public:
		void CompileFrom(const std::wstring& path, const std::string& vertexName, const std::string& pixelName)
		{
			D3DX11CompileFromFile(path.c_str(), 0, 0, vertexName.c_str(), "vs_4_0", 0, 0, 0, &m_VBlob, 0, 0);
			D3DX11CompileFromFile(path.c_str(), 0, 0, pixelName.c_str(), "ps_4_0", 0, 0, 0, &m_PBlob, 0, 0);

			DXEngine::Instance->Device()->CreateVertexShader(m_VBlob->GetBufferPointer(), m_VBlob->GetBufferSize(), NULL, &m_VShader);
			DXEngine::Instance->Device()->CreatePixelShader(m_PBlob->GetBufferPointer(), m_PBlob->GetBufferSize(), NULL, &m_PShader);
		}
		void Active()
		{
			DXEngine::Instance->Devcon()->VSSetShader(m_VShader, 0, 0);
			DXEngine::Instance->Devcon()->PSSetShader(m_PShader, 0, 0);
		}
		void Release()
		{
			m_VShader->Release();
			m_PShader->Release();
			m_VBlob->Release();
			m_PBlob->Release();
		}
		ID3D11VertexShader* GetVShader()
		{
			return m_VShader;
		}
		ID3D11PixelShader* GetPShader()
		{
			return m_PShader;
		}
		ID3D10Blob* GetVBlob()
		{
			return m_VBlob;
		}
		ID3D10Blob* GetPBlob()
		{
			return m_PBlob;
		}
	};

	template < typename TVertex>
	class VertexBuffer
	{
	private:
		ID3D11Buffer *m_Buffer;
		UINT m_Size;

	public:
		void CreateBuffer(UINT bufferSize)
		{
			m_Size = bufferSize;

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));

			bd.Usage = D3D11_USAGE_DYNAMIC;                // Write access access by CPU and GPU
			bd.ByteWidth = sizeof(TVertex) * m_Size;    // Size is the VERTEX struct
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // Use as a vertex vertexBuffer
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // Allow CPU to write in vertexBuffer

			DXEngine::Instance->Device()->CreateBuffer(&bd, NULL, &m_Buffer);    // Create the vertexBuffer
		}
		void Fill(TVertex* source, D3D11_MAP mapMode = D3D11_MAP::D3D11_MAP_WRITE_DISCARD)
		{
			D3D11_MAPPED_SUBRESOURCE ms;
			DXEngine::Instance->Devcon()->Map(m_Buffer, NULL, mapMode, NULL, &ms);
			memcpy(ms.pData, source, m_Size * sizeof(TVertex));
			DXEngine::Instance->Devcon()->Unmap(m_Buffer, NULL);
		}
		ID3D11Buffer*& GetBuffer()
		{
			return m_Buffer;
		}
		UINT GetSize()
		{
			return m_Size;
		}
		void Release()
		{
			m_Buffer->Release();
		}
	};


	template < UINT PARAMETERS >
	class InputLayout
	{
	private:
		ID3D11InputLayout *m_Layout;

	public:
		struct Layout
		{
			LPCSTR semantic;
			DXGI_FORMAT format;
		};
		void CreateInputLayout(const std::array<Layout, PARAMETERS>& semantics, Shader& shader)
		{
			D3D11_INPUT_ELEMENT_DESC ied[PARAMETERS];

			int countPOSITION = 0;
			int countPOSITIONT = 0;
			int countCOLOR = 0;
			int countPSIZE = 0;

			for (int i = 0; i < PARAMETERS; ++i)
			{
				UINT semanticIndex = 0;
				if (strcmp(semantics[i].semantic, "POSITION") == 0)
				{
					semanticIndex = countPOSITION;
					++countPOSITION;
				}
				if (strcmp(semantics[i].semantic, "POSITIONT") == 0)
				{
					semanticIndex = countPOSITIONT;
					++countPOSITIONT;
				}
				if (strcmp(semantics[i].semantic, "COLOR") == 0)
				{
					semanticIndex = countCOLOR;
					++countCOLOR;
				}
				if (strcmp(semantics[i].semantic, "PSIZE") == 0)
				{
					semanticIndex = countPSIZE;
					++countPSIZE;
				}
				ied[i] = { semantics[i].semantic, semanticIndex, semantics[i].format, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			}

			DXEngine::Instance->Device()->CreateInputLayout(ied, PARAMETERS, shader.GetVBlob()->GetBufferPointer(), shader.GetVBlob()->GetBufferSize(), &m_Layout);
		}
		void Active()
		{
			DXEngine::Instance->Devcon()->IASetInputLayout(m_Layout);
		}
		void Release()
		{
			m_Layout->Release();
		}
	};

	class Renderer
	{
	private:
		D3D11_PRIMITIVE_TOPOLOGY m_Topology = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	public:
		void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
		{
			m_Topology = topology;
		}

		template < typename TVertex >
		void Render(VertexBuffer<TVertex>& vertexBuffer)
		{
			UINT stride = sizeof(TVertex);
			UINT offset = 0;
			DXEngine::Instance->Devcon()->IASetVertexBuffers(0, 1, &(vertexBuffer.GetBuffer()), &stride, &offset);
			DXEngine::Instance->Devcon()->IASetPrimitiveTopology(m_Topology);
			DXEngine::Instance->Devcon()->Draw(vertexBuffer.GetSize(), 0);
		}
	};

	template < typename TVertex >
	class Shape
	{
		std::vector<TVertex> vertices;
	};
}

//------- Useful utilities ---------------------------------------------------------------------------

/* Multy-threading */

template<class T, class U>
struct is_same_function : std::false_type {};
template<class T>
struct is_same_function<T, T> : std::true_type {};

///<summary>Create async corutine</summary>
///<param name="Routine">Reference to function with signature: void(args...). DON'T CALL DRAW FUNCS IN COROUTINE!!!</param>
///<param name="Inst">Pointer to calling class (this)</param>
///<param name="Args">Args that match to function signature</param>
template < typename Foo, typename CE_Class, typename ... In_Params  >
static void StartCoroutine(Foo Routine, CE_Class* Inst, In_Params... Args)
{
	static_assert(is_same_function< Foo, void(__thiscall CE_Class::*)(In_Params...)>::value,
				  "Foo must have void(args...) signature, please check return type and args match");
	std::thread(Routine, Inst, Args...).detach();
}

static void WaitFor(std::chrono::milliseconds Msec) { std::this_thread::sleep_for(Msec); }
static void WaitFor(std::chrono::seconds Sec) { std::this_thread::sleep_for(Sec); }

// Just a timer
class Timer
{
private:
	std::chrono::milliseconds m_CountdownTime = 1000ms;
	std::chrono::time_point<std::chrono::steady_clock> m_TimerStartPoint;

public:
	Timer() { m_TimerStartPoint = std::chrono::steady_clock::now(); }
	Timer(std::chrono::milliseconds Ms) : m_CountdownTime(Ms) {}

	void SetTimer(std::chrono::milliseconds Ms) { m_CountdownTime = Ms;  m_TimerStartPoint = std::chrono::steady_clock::now(); }

	void Start()
	{
		m_TimerStartPoint = std::chrono::steady_clock::now();
	}

	void Stop()
	{
		m_TimerStartPoint = m_CountdownTime + std::chrono::steady_clock::now();
	}

	double Time() const
	{
		double TimeRemaining = std::chrono::duration<double>(m_TimerStartPoint + m_CountdownTime - std::chrono::steady_clock::now()).count();
		if (TimeRemaining > 0.0)
			return TimeRemaining;
		else
			return 0.0;
	}

	bool Ready() const
	{
		return Time() == 0.0;
	}
};