#pragma once
#include "MouseEvent.h"
#include <queue>

class Mouse
{
public:
	struct ButtonState
	{
		bool Pressed = false;
		bool Released = false;
		bool Held = false;
	};

	Mouse();
	const ButtonState& GetButton(unsigned char button) const;
	const ButtonState& GetButton(BUTTON button) const;
	const stf::iVec2& GetPosition() const;
	const stf::iVec2& GetDelta() const;
	const int GetWheel() const;
	const int GetWheelUp() const;
	const int GetWheelDown() const;
	void ResetStates();

	void EnableBuffering();
	void DisableBuffering();
	bool IsBufferingEnabled() const;
	bool IsEventBufferEmpty() const;
	MouseEvent PeekEvent();
	void PopEvent();

	void OnRawInput(stf::iVec2 delta);
	void OnButtonEvent(MouseEvent mouseEvent);
	void OnMoveEvent(stf::iVec2 position);
	void OnWheelEvent(int wheel, stf::iVec2 position);

private:
	ButtonState buttonStates[5];
	stf::iVec2 position;
	stf::iVec2 deltaPosition;
	int wheel;
	std::queue<MouseEvent> eventBuffer;
	bool bufferingEnabale = false;
};

