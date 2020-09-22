#include "Mouse.h"

Mouse::Mouse()
	: position({ 0, 0 }), deltaPosition({ 0, 0 }), wheel(0)
{
}

const Mouse::ButtonState & Mouse::GetButton(unsigned char button) const
{
	return this->buttonStates[button];
}

const Mouse::ButtonState & Mouse::GetButton(BUTTON button) const
{
	return this->buttonStates[static_cast<unsigned char>(button)];
}

const stf::iVec2 & Mouse::GetPosition() const
{
	return this->position;
}

const stf::iVec2 & Mouse::GetDelta() const
{
	return this->deltaPosition;
}

const int Mouse::GetWheel() const
{
	return this->wheel;
}

const int Mouse::GetWheelUp() const
{
	return this->wheel * (wheel >= 0);
}

const int Mouse::GetWheelDown() const
{
	return -(this->wheel) * (wheel <= 0);
}

void Mouse::ResetStates()
{
	for (ButtonState button : this->buttonStates)
	{
		button.Pressed = false;
		button.Released = false;
	}
	this->deltaPosition = { 0, 0 };
	this->wheel = 0;
}

void Mouse::EnableBuffering()
{
	this->bufferingEnabale = true;
}

void Mouse::DisableBuffering()
{
	this->bufferingEnabale = false;
}

bool Mouse::IsBufferingEnabled() const
{
	return this->bufferingEnabale;
}

bool Mouse::IsEventBufferEmpty() const
{
	return this->eventBuffer.empty();
}

MouseEvent Mouse::PeekEvent()
{
	if (this->eventBuffer.empty())
	{
		return MouseEvent();
	}
	else
	{
		return this->eventBuffer.front();
	}
}

void Mouse::PopEvent()
{
	this->eventBuffer.pop();
}

void Mouse::OnRawInput(stf::iVec2 delta)
{
	this->deltaPosition = delta;

	if (this->bufferingEnabale)
	{
		this->eventBuffer.push(MouseEvent(MouseEvent::RawInput, delta, BUTTON::NOT_BUTTON));
	}
}

void Mouse::OnButtonEvent(MouseEvent mouseEvent)
{
	unsigned char buttonCode = static_cast<unsigned char>(mouseEvent.Button());
	this->buttonStates[buttonCode].Pressed = false;
	this->buttonStates[buttonCode].Released = false;
	if (mouseEvent.Type() == MouseEvent::Press)
	{
		this->buttonStates[buttonCode].Pressed = !this->buttonStates[buttonCode].Held;
		this->buttonStates[buttonCode].Held = true;
	}
	else
	{
		this->buttonStates[buttonCode].Released = true;
		this->buttonStates[buttonCode].Held = false;
	}

	if (this->bufferingEnabale)
	{
		this->eventBuffer.push(mouseEvent);
	}
}

void Mouse::OnMoveEvent(stf::iVec2 position)
{
	this->position = position;

	if (this->bufferingEnabale)
	{
		this->eventBuffer.push(MouseEvent(MouseEvent::Move, position, BUTTON::NOT_BUTTON));
	}
}

void Mouse::OnWheelEvent(int wheel, stf::iVec2 position)
{
	this->wheel = wheel;
	MouseEvent::EventType eventType = MouseEvent::EventType((wheel > 0) * MouseEvent::WheelUp + (wheel < 0) * MouseEvent::WheelDown);

	if (this->bufferingEnabale)
	{
		this->eventBuffer.push(MouseEvent(eventType, position, BUTTON::NOT_BUTTON));
	}
}