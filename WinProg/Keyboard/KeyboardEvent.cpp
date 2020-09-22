#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent()
	: type(EventType::Invalid), key(0U) 
{
}

KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key)
	: type(type),  key(key)
{
}

bool KeyboardEvent::IsPressed()
{
	return this->type == EventType::Press;
}

bool KeyboardEvent::IsRepeated()
{
	return this->type == EventType::Repeat;
}

bool KeyboardEvent::IsReleased()
{
	return this->type == EventType::Release;
}

bool KeyboardEvent::IsValid()
{
	return this->type != EventType::Invalid;
}

unsigned char KeyboardEvent::Key()
{
	return this->key;
}
