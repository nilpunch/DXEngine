#include "MouseEvent.h"

MouseEvent::MouseEvent()
	: type(EventType::Invalid), position(0, 0)
{
}

MouseEvent::MouseEvent(EventType type, stf::iVec2 position, BUTTON button)
	: type(type), position(position), button(button)
{
}

MouseEvent::EventType MouseEvent::Type() const
{
	return this->type;
}

BUTTON MouseEvent::Button() const
{
	return button;
}

stf::iVec2 MouseEvent::Position() const
{
	return this->position;
}

bool MouseEvent::IsValid() const
{
	return this->type != EventType::Invalid;
}