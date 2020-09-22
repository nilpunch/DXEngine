#pragma once
#include "KeyboardEvent.h"
#include <queue>

class Keyboard
{
public:
	struct KeyState
	{
		bool Pressed = false;
		bool Released = false;
		bool Held = false;
	};

	const KeyState& GetKey(const unsigned char key) const;
	const KeyState& GetKey(const KEY key) const;
	void ResetStates();

	void EnableCharBuffering();
	void DisableCharBuffering();
	bool IsCharBufferingEnabled() const;

	void EnableKeyBuffering();
	void DisableKeyBuffering();
	bool IsKeyBufferingEnabled() const;

	bool IsCharsBufferEmpty() const;
	bool IsKeysBufferEmpty() const;
	KeyboardEvent PeekKeyEvent();
	KeyboardEvent PeekCharEvent();
	void PopKeyEvent();
	void PopCharEvent();

	void OnCharEvent(unsigned char key);
	void OnKeyEvent(KeyboardEvent kbEvent);

	void EnableAutoRepeatKeys();
	void EnableAutoRepeatChars();
	void DisableAutoRepeatKeys();
	void DisableAutoRepeatChars();
	bool IsAutoRepeatKeys() const;
	bool IsAutoRepeatChars() const;

private:
	bool autoRepeatKeys = false;
	bool autoRepeatChars = true;
	KeyState keyStates[256];
	std::queue<KeyboardEvent> keyEventBuffer;
	std::queue<KeyboardEvent> charEventBuffer;
	bool keyBufferingEnable = false;
	bool charBufferingEnable = false;
};