#include "Keyboard.h"

const Keyboard::KeyState & Keyboard::GetKey(const unsigned char key) const
{
	return this->keyStates[key];
}

const Keyboard::KeyState & Keyboard::GetKey(KEY key) const
{
	return this->keyStates[static_cast<unsigned char>(key)];
}

void Keyboard::ResetStates()
{
	for (KeyState& key : this->keyStates)
	{
		key.Pressed = false;
		key.Released = false;
	}
}

void Keyboard::EnableKeyBuffering()
{
	this->keyBufferingEnable = true;
}

void Keyboard::DisableKeyBuffering()
{
	this->keyBufferingEnable = false;
}

bool Keyboard::IsKeyBufferingEnabled() const
{
	return this->keyBufferingEnable;
}

void Keyboard::EnableCharBuffering()
{
	this->charBufferingEnable = true;
}

void Keyboard::DisableCharBuffering()
{
	this->charBufferingEnable = false;
}

bool Keyboard::IsCharBufferingEnabled() const
{
	return this->charBufferingEnable;
}

bool Keyboard::IsKeysBufferEmpty() const
{
	return this->keyEventBuffer.empty();
}

bool Keyboard::IsCharsBufferEmpty() const
{
	return this->charEventBuffer.empty();
}

KeyboardEvent Keyboard::PeekKeyEvent()
{
	if (this->keyEventBuffer.empty())
	{
		return KeyboardEvent();
	}
	else
	{
		return this->keyEventBuffer.front();
	}
}

KeyboardEvent Keyboard::PeekCharEvent()
{
	if (this->charEventBuffer.empty())
	{
		return KeyboardEvent();
	}
	else
	{
		return this->charEventBuffer.front();
	}
}

void Keyboard::PopKeyEvent()
{
	this->keyEventBuffer.pop();
}

void Keyboard::PopCharEvent()
{
	this->charEventBuffer.pop();
}

void Keyboard::OnCharEvent(unsigned char key)
{
	if (this->charBufferingEnable)
	{
		this->charEventBuffer.push(KeyboardEvent(KeyboardEvent::Press, key));
	}
}

void Keyboard::OnKeyEvent(KeyboardEvent keyEvent)
{
	unsigned char keyCode = keyEvent.Key();
	this->keyStates[keyCode].Pressed = false;
	this->keyStates[keyCode].Released = false;
	if (keyEvent.IsPressed() || keyEvent.IsRepeated())
	{
		this->keyStates[keyCode].Pressed = !this->keyStates[keyCode].Held;
		this->keyStates[keyCode].Held = true;
	}
	else
	{
		this->keyStates[keyCode].Released = true;
		this->keyStates[keyCode].Held = false;
	}

	if (this->keyBufferingEnable)
	{
		this->keyEventBuffer.push(keyEvent);
	}
}

void Keyboard::EnableAutoRepeatKeys()
{
	this->autoRepeatKeys = true;
}

void Keyboard::EnableAutoRepeatChars()
{
	this->autoRepeatChars = true;
}

void Keyboard::DisableAutoRepeatKeys()
{
	this->autoRepeatKeys = false;
}

void Keyboard::DisableAutoRepeatChars()
{
	this->autoRepeatChars = false;
}

bool Keyboard::IsAutoRepeatKeys() const
{
	return this->autoRepeatKeys;
}

bool Keyboard::IsAutoRepeatChars() const
{
	return this->autoRepeatChars;
}
