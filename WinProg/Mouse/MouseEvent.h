#pragma once
#include <stf/Vector.h>

enum class BUTTON : unsigned char
{
	NOT_BUTTON,
	LEFT,
	RIGHT,
	MIDDLE
};

class MouseEvent
{
public:
	enum EventType
	{
		Release,
		Press,
		Move,
		WheelUp,
		WheelDown,
		RawInput,
		Invalid
	};

	MouseEvent();
	MouseEvent(EventType type, stf::iVec2 position, BUTTON button);
	EventType Type() const;
	BUTTON Button() const;
	stf::iVec2 Position() const;
	bool IsValid() const;

private:
	EventType type;
	BUTTON button;
	stf::iVec2 position;
};

