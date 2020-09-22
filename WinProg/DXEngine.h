#pragma once
#include "WindowContainer.h"

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <windowsx.h>

#include <string>
#include <vector>
#include <array>
#include <assert.h>

#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

//#include <stf/Random.h>
//#include <stf/Vector.h>
//#include <stf/Matrix.h>

#define PI 3.141592653589793
#define TWO_PI 6.283185307179586
#define HALF_PI 1.570796326794896

// #define WE_NO_FPS_LIMIT // Uncomment this to unlock framerate
#define DXE_DEFAULT_FPS_LIM 60.0
#define DXE_MOUSE_MAX_BUTTONS 5
#define DXE_AVERAGE_FRAMELIST_SIZE 10

// In case this is wheel roll up and down event
#define DXE_MOUSE_ADDITIONAL_EVENTS 2

// In this namespace defined a lot of cool (my own) usefull classes 
using namespace stf;

// Using time literals
using namespace std::literals::chrono_literals;

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

/* You must publicly inheritated from this */
class DXEngine : public WindowContainer
{
public:
	bool InitialiseWindow(HINSTANCE hInstance, const std::wstring& title, iVec2 size, iVec2 position, const std::wstring& className = L"WindowClass1");
	bool InitialiseWindow(HINSTANCE hInstance, const std::wstring & title, iVec2 size);
	void Start();

protected:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

private:
	void MessagesMainThreadLoop();
	void StableGameThreadLoop();

protected:
	void Quit();

private:
	bool activeMainLoop = false;

protected:
	const float DeltaTime() const;
	const float RunTime() const;
	void SetFramerate(float framerate);

private:
	std::chrono::duration<long long, std::ratio_multiply<std::hecto, std::nano>> framerate =
		std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<float>{ 1.0f / DXE_DEFAULT_FPS_LIM });
	float stableDeltaTime = 0.0f;
	std::chrono::system_clock::time_point tpGameStart;
};