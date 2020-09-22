#include "DXEngine.h"

bool DXEngine::InitialiseWindow(HINSTANCE hInstance, const std::wstring & title, iVec2 size, iVec2 position, const std::wstring & className)
{
	if (!window.Initialise(this, hInstance, title, className, size, position))
		return false;

	if (!gfx.Initialise(window.GetHWND(), size.x, size.y))
		return false;

	return true;
}

bool DXEngine::InitialiseWindow(HINSTANCE hInstance, const std::wstring & title, iVec2 size)
{
	stf::iVec2 screenCenter = (stf::iVec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) - size) / 2;
	return DXEngine::InitialiseWindow(hInstance, title, size, screenCenter);
}

void DXEngine::Start()
{
	activeMainLoop = true;
	std::thread thGame = std::thread(&DXEngine::StableGameThreadLoop, this);
	this->MessagesMainThreadLoop();
	thGame.join();
}

void DXEngine::MessagesMainThreadLoop()
{
	while (window.ProcessMessages() && activeMainLoop);
	Quit();
}

void DXEngine::StableGameThreadLoop()
{
	this->Init();
	
#ifndef DXE_NO_FPS_LIMIT
	auto tpBeginFrame = std::chrono::high_resolution_clock::now();
	auto tpEndFrame = tpBeginFrame + framerate;
#endif
	auto tpPrevTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
	std::chrono::duration<float> tpElapsedTime;


	while (activeMainLoop)
	{
		gfx.RenderPreparation();

		std::unique_lock<std::mutex> lockInput(inputLock);

		this->Update();

		this->keyboard.ResetStates();
		this->mouse.ResetStates();
		lockInput.unlock();

		this->Draw();

#ifndef DXE_NO_FPS_LIMIT
		auto tpCurrentTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now());
		if (tpCurrentTime < tpEndFrame)
		{
			std::this_thread::sleep_until(tpEndFrame);
			tpBeginFrame = tpEndFrame;
		}
		else
		{
			tpBeginFrame = tpCurrentTime;
		}
		tpEndFrame = tpBeginFrame + this->framerate;
#endif
		tpElapsedTime = tpCurrentTime - tpPrevTime;
		tpPrevTime = tpCurrentTime;
		this->stableDeltaTime = tpElapsedTime.count();

		this->gfx.SwapChain();
	}
}

void DXEngine::Quit()
{
	activeMainLoop = false;
}

const float DXEngine::DeltaTime() const
{
	return stableDeltaTime;
}

const float DXEngine::RunTime() const
{
	return std::chrono::duration<float>(std::chrono::system_clock::now() - tpGameStart).count();
}

void DXEngine::SetFramerate(float framerate)
{
	this->framerate = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<float>{ 1.0f / framerate });
}
